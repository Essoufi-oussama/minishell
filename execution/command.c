/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:12:09 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/11 15:47:47 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exit_status(int status, t_data *data)
{
	if (WIFSIGNALED(status))
		data->exit_status = 128 + WTERMSIG(status);
	else if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
}

int	execute_child_process(char **str, char **env_list,
		t_data *data, t_alloc **head)
{
	char	*path;

	files(data->commands[0], data, head, 0);
	check_if_building(str, data, head);
	path = get_path(env_list, str[0], data, head);
	if (!path)
	{
		write(2, "minihell: ", 11);
		if (access(str[0], F_OK) == -1)
		{
			ft_putstr_fd(str[0], 2);
			ft_putstr_fd(": command not found\n", 2);
		}
		else
			perror(str[0]);
		free_exit_child(data, head, 127);
	}
	if (execve(path, str, env_list) == -1)
	{
		(access(str[0], F_OK | X_OK),
			perror(str[0]));
		free_exit_child(data, head, 1);
	}
	return (0);
}

int	execu_cmd(char **str, char **env_list, t_data *data)
{
	int		pid;
	int		status;
	t_alloc	*head;

	pid = fork();
	if (pid < 0)
		free_exit(data);
	if (pid == 0)
	{
		head = NULL;
		execute_child_process(str, env_list, data, &head);
	}
	else
	{
		wait(&status);
		exit_status(status, data);
	}
	return (0);
}

void	execution_cas(char **args, t_env *env_list, t_data *data)
{
	char	**envs;
	t_alloc	*head;

	head = NULL;
	files(data->commands[0], data, &head, 1);
	envs = convert_env_list_to_array(env_list, data);
	if (ft_strcmp(args[0], "cd") == 0)
		cd(args, data);
	else if (ft_strcmp(args[0], "exit") == 0)
		exit_program(args, data);
	else if (ft_strcmp(args[0], "export") == 0 && args[1] != NULL)
		export(args, data);
	else if (ft_strcmp(args[0], "unset") == 0)
		unset(args, data);
	else
		execu_cmd(args, envs, data);
}

int	execute(t_data *data)
{
	t_command	*cmd;

	if (data->command_count == 1)
	{
		cmd = data->commands[0];
		execution_cas(cmd->args, data->env, data);
	}
	else
		pipe_cas(data->commands, data->env, data);
	return (0);
}
