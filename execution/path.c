/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:31:32 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/02/20 13:31:34 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


char	**env_path(char **env)
{
	int		i;
	char	**path;

	path = NULL;
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path = ft_split(env[i] + 5, ':');
			if (!path)
				return (NULL);
			break ;
		}
		i++;
	}
	return (path);
}

char	*find_path(char **path, char *cmd)
{
	int		i;
	char	*path1;
	char	*final_path;

	i = 0;
	while (path[i])
	{
		path1 = ft_strjoin(path[i], "/");
		if (!path1)
			return (NULL);
		final_path = ft_strjoin(path1, cmd);
		free(path1);
		if (!final_path)
			return (NULL);
		if (access(final_path, F_OK | X_OK) == 0)
			return (final_path);
		free(final_path);
		i++;
	}
	return (NULL);
}

char	*direct_execution(char **cmd, char **path)
{
	char	*dup_cmp;

	if (access(cmd[0], F_OK | X_OK) == 0)
	{
		dup_cmp = ft_strdup(cmd[0]);
		if (!dup_cmp)
			return (NULL);
		free_ultimate(path);
		free_ultimate(cmd);
		return (dup_cmp);
	}
	return (NULL);
}

char	*get_path(char **env, char *cmp)
{
	char	**path;
	char	**cmd;
	char	*final_path;
	int		i;

	i = 0;
	path = env_path(env);
	if (!path)
		return (NULL);
	cmd = ft_split(cmp, ' ');
	if (!cmd)
		return (free_ultimate(path), NULL);
	if (cmp[0] == '.' && cmp[1] == '/')
		return (direct_execution(cmd, path));
	final_path = find_path(path, cmd[0]);
	if (final_path)
		return (free_ultimate(cmd), free_ultimate(path), final_path);
	while (cmp[i])
	{
		if (cmp[i] == '/')
			return (direct_execution(cmd, path));
		i++;
	}
	(free_ultimate(cmd), free_ultimate(path));
	return (NULL);
}
