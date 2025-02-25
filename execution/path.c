/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:31:32 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/02/24 17:25:17 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**env_path(char **env, t_data *data)
{
	int		i;
	char	**path;

	path = NULL;
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path = ft_split(env[i] + 5, ':', data);
			if (!path)
				return (NULL);
			break ;
		}
		i++;
	}
	return (path);
}

char	*find_path(char **path, char *cmd, t_data *data)
{
	int		i;
	char	*path1;
	char	*final_path;

	i = 0;
	while (path[i])
	{
		path1 = ft_strjoin(path[i], "/", data);
		final_path = ft_strjoin(path1, cmd, data);
		if (access(final_path, F_OK | X_OK) == 0)
			return (final_path);
		i++;
	}
	return (NULL);
}

char	*direct_execution(char *cmd, char **path, t_data *data)
{
	char	*dup_cmp;

	(void)path;
	if (access(cmd, F_OK | X_OK) == 0)
	{
		dup_cmp = ft_strdup(cmd, data);
		return (dup_cmp);
	}
	return (NULL);
}

char	*get_path(char **env, char *cmp, t_data *data)
{
	char	**path;
	char	*final_path;
	int		i;

	i = 0;
	path = env_path(env, data);
	if (cmp[0] == '.' && cmp[1] == '/')
		return (direct_execution(cmp, path,data));
	final_path = find_path(path, cmp,data);
	if (final_path)
		return (final_path);
	while (cmp[i])
	{
		if (cmp[i] == '/')
			return (direct_execution(cmp, path,data));
		i++;
	}
	return (NULL);
}
