/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   building_command_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 20:23:45 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/13 20:58:35 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*search_path(char **env)
{
	int	i;

	if (env == NULL)
		ft_error("No environment");
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

int	check_in_directory(char	*cmd, char **cmds)
{
	if (cmd == NULL)
		return (0);
	if (ft_strlen(cmd) > 2)
	{
		if (cmd[0] == '.' && cmd[1] == '/' )
		{
			if (access(cmd, F_OK | X_OK) == -1)
			{
				perror(cmd);
				clear_split(cmds);
				exit (1);
			}
			else
				return (1);
		}
	}
	return (0);
}

char	*is_valid_command_name(char *cmd, char **paths)
{
	int		i;
	char	*slash;
	char	*command;

	i = 0;
	while (paths[i])
	{
		slash = ft_strjoin(paths[i], "/");
		if (slash == NULL)
			ft_error("error when allocating slash\n");
		command = ft_strjoin(slash, cmd);
		free(slash);
		if (command == NULL)
			ft_error("error when allocating command with slash\n");
		if (access(command, F_OK | X_OK) != -1)
			return (command);
		free(command);
		i++;
	}
	access(cmd, F_OK | X_OK);
	return (NULL);
}

int	is_valid_absolute_path(char *argv, char **str)
{
	if (argv[0] == '/')
	{
		if (access(argv, F_OK | X_OK) != 0)
		{
			*str = NULL;
			return (1);
		}
		else
		{
			*str = ft_strdup(argv);
			return (1);
		}
	}
	return (0);
}

char	*build_command(char *path, char **cmds, char *argv_cmd)
{
	char	**paths;
	char	*str;

	if (check_in_directory(argv_cmd, cmds))
	{
		str = ft_strdup(argv_cmd);
		if (str == NULL)
		{
			clear_split(cmds);
			ft_error("error during allocation");
		}
		return (str);
	}
	if (is_valid_absolute_path(argv_cmd, &str))
		return (str);
	paths = ft_split(path, ':');
	if (paths == NULL)
	{
		clear_split(cmds);
		ft_error("error when splitting paths\n");
	}
	str = is_valid_command_name(argv_cmd, paths);
	clear_split(paths);
	return (str);
}
