/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 11:53:25 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/13 20:55:37 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	child_process(char **argv, char *path, int *p, char **env)
{
	int	fd;

	fd = open(argv[1], O_RDONLY);
	close(p[0]);
	if (fd == -1)
	{
		close(p[1]);
		perror(argv[1]);
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, 0) == -1)
	{
		close(fd);
		close(p[1]);
		ft_error("dup2 in first child failed");
	}
	close(fd);
	if (dup2(p[1], 1) == -1)
	{
		close(fd);
		close(p[1]);
		ft_error("dup2 in first child failed");
	}
	close(p[1]);
	execute_program(argv[2], path, env);
}

void	child_process2(char **argv, char *path, int *p, char **env)
{
	int	fd;

	close(p[1]);
	fd = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY, 0777);
	if (fd == -1)
	{
		close(p[0]);
		perror("error");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, 1) == -1)
	{
		close(fd);
		close(p[0]);
		ft_error("dup 2 of child 2 failed");
	}
	if (dup2(p[0], 0) == -1)
	{
		close(fd);
		close(p[0]);
		ft_error("dup 2 of child 2 failed");
	}
	close(fd);
	close(p[0]);
	execute_program(argv[3], path, env);
}

int	main(int argc, char **argv, char **env)
{
	pid_t	pid;
	pid_t	pid2;
	int		p[2];
	char	*path;

	if (argc != 5)
		ft_error("wrong number of args\n");
	if (pipe(p) == -1)
		ft_error("pipe failed\n");
	path = search_path(env);
	pid = fork();
	if (pid == -1)
		close_and_exit_error(p);
	if (pid == 0)
		child_process(argv, path, p, env);
	else
	{
		pid2 = fork();
		if (pid2 == -1)
			close_and_exit_error(p);
		if (pid2 == 0)
			child_process2(argv, path, p, env);
	}
	close_and_exit_success(p, &pid, &pid2);
	return (0);
}
