/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 12:12:59 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/08 12:47:43 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	execute_program(char *argv_cmd, char *path, char **env)
{
	char	*cmd;
	char	**child_argv;

	child_argv = ft_split(argv_cmd, ' ');
	if (child_argv == NULL)
		ft_error("Error when creating child args\n");
	if (child_argv[0] == NULL)
	{
		free(child_argv);
		ft_error("Empty command");
	}
	cmd = build_command(path, child_argv, child_argv[0]);
	if (cmd == NULL)
	{
		clear_split(child_argv);
		perror(child_argv[0]);
		exit(EXIT_FAILURE);
	}
	free(child_argv[0]);
	child_argv[0] = cmd;
	if (execve(cmd, child_argv, env) == -1)
		clear_split(child_argv);
	exit (1);
}

void	child_here_doc(int *p, char *limiter)
{
	char	*line;

	close(p[0]);
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (!line || ((ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
					&& (ft_strlen(line) - 1) == ft_strlen(limiter))))
		{
			free(line);
			break ;
		}
		if (write(p[1], line, ft_strlen(line)) == -1)
		{
			free(line);
			ft_error_sys("error");
		}
		free(line);
	}
	get_next_line(-2);
	close(p[1]);
	exit(EXIT_SUCCESS);
}

int	handle_here_doc(char **argv, int *flag, int argc)
{
	int		p[2];
	pid_t	pid;

	if (argc < 6)
		ft_error("Wrong number of arguments");
	*flag = 1;
	if (pipe(p) == -1)
		ft_error_sys("pipe");
	pid = fork();
	if (pid == -1)
		close_and_exit_error(p);
	if (pid == 0)
		child_here_doc(p, argv[2]);
	else
	{
		wait(NULL);
		close(p[1]);
	}
	return (p[0]);
}

void	last_child_here_doc(char **argv, char *path, int last_p, char **env)
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
		fd = open(argv[1], O_CREAT | O_WRONLY | O_APPEND, 0777);
		if (fd == -1)
		{
			close(last_p);
			ft_error_sys(argv[1]);
		}
		prepare_child(last_p, fd);
		execute_program(argv[0], path, env);
	}
}
