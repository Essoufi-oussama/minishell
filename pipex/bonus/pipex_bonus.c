/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 20:31:48 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/05 14:27:47 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	prepare_child(int input, int output)
{
	if (dup2(input, 0) == -1)
	{
		close(input);
		close(output);
		ft_error_sys("error");
	}
	close(input);
	if (dup2(output, 1) == -1)
	{
		close(output);
		ft_error_sys("error");
	}
	close(output);
}

int	first_children(char **argv, char *path, char **env)
{
	int		fd;
	int		p[2];
	pid_t	pid;

	if (pipe(p) == -1)
		ft_error_sys("error");
	pid = fork();
	if (pid == -1)
		close_and_exit_error(p);
	if (pid == 0)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
			close_and_exit_error(p);
		close(p[0]);
		prepare_child(fd, p[1]);
		execute_program(argv[2], path, env);
	}
	else
		close(p[1]);
	return (p[0]);
}

int	create_childrens(int last_p, char *cmd, char *path, char **env)
{
	pid_t	pid;
	int		child_p[2];

	if (pipe(child_p) == -1)
		ft_error2("Error\n", last_p);
	pid = fork();
	if (pid == -1)
	{
		close(last_p);
		close_and_exit_error(child_p);
	}
	if (pid == 0)
	{
		close(child_p[0]);
		prepare_child(last_p, child_p[1]);
		execute_program(cmd, path, env);
	}
	close(child_p[1]);
	close(last_p);
	return (child_p[0]);
}

void	last_child(char **argv, char *path, int last_p, char **env)
{
	int		fd;
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("error");
		close(last_p);
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0777);
		if (fd == -1)
		{
			close(last_p);
			ft_error(argv[1]);
		}
		prepare_child(last_p, fd);
		execute_program(argv[0], path, env);
	}
}

int	main(int argc, char **argv, char **env)
{
	char	*path;
	int		last_pipe;
	int		i;
	int		flag;

	flag = 0;
	if (argc < 5)
		ft_error("wrong_number_of arguments");
	path = search_path(env);
	if (ft_strncmp(argv[1], "here_doc", 8) == 0 && ft_strlen(argv[1]) == 8)
		last_pipe = handle_here_doc(argv, &flag, argc);
	else
		last_pipe = first_children(argv, path, env);
	i = 3;
	while (i < argc - 2)
	{
		last_pipe = create_childrens(last_pipe, argv[i], path, env);
		i++;
	}
	if (flag)
		last_child_here_doc(argv + i, path, last_pipe, env);
	else
		last_child(argv + i, path, last_pipe, env);
	close_and_exit_success(last_pipe);
}
