/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 02:43:05 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/11 15:42:12 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (fd < 0 || s == NULL)
		return ;
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
}

void	handle_cmd_error(char *cmd, t_data *data, t_alloc **head)
{
	if (access(cmd, F_OK) == -1)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	else
		perror(cmd);
	free_exit_child(data, head, 127);
}

void	execute_command(char **argv, char **env,
		t_data *data, t_command *command)
{
	char	*path;
	t_alloc	*head;

	head = NULL;
	files(command, data, &head,0);
	check_if_building(argv, data, &head);
	path = get_path(env, argv[0], data, &head);
	if (!path)
		handle_cmd_error(argv[0], data, &head);
	if (execve(path, argv, env) == -1)
	{
		access(argv[0], F_OK | X_OK);
		perror(argv[0]);
		free_exit_child(data, &head, 1);
	}
}

void	ft_dup2(int input, int output, t_data *data)
{
	if (dup2(input, 0) == -1)
	{
		perror("dup2");
		close(input);
		close(output);
		free_exit2(data, 1);
	}
	close(input);
	if (dup2(output, 1) == -1)
	{
		perror("dup2");
		close(output);
		free_exit2(data, 1);
	}
	close(output);
}

void	free_exit_child(t_data *data, t_alloc **head, int i)
{
	ft_lstclear_garbage(&data->alloc);
	ft_lstclear_env(&data->env);
    if (data->default_path)
		free(data->default_path);
	free(data);
	ft_lstclear_garbage(head);
	exit(i);
}
