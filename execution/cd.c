/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:22:33 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/12 18:01:48 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_home(t_env *env)
{
	while (env != NULL)
	{
		if (ft_strncmp(env->env_var, "HOME=", 5) == 0)
			return (env->env_var + 5);
		env = env->next;
	}
	return (NULL);
}

static int	update_pwd_and_oldpwd(t_data *data, char *path3, char *prev_wd)
{
	char	cwd[PATH_MAX];
	char	*path;
	char	*path2;
	int flag = 0;

	if (ft_getenv2("PWD", data) == NULL && ft_getenv2("OLDPWD", data) != NULL)
	{
		remove_env_var("OLDPWD", data);
		check_add("OLDPWD", data);
		flag = 1;
	}
	else
		check_add(ft_strjoin("OLDPWD=", prev_wd, data), data);
	path = getcwd(cwd, PATH_MAX);
	if (!path)
	{
		perror("cd: error retrieving current directory:"
			"getcwd: cannot access parent directories");
		if (prev_wd == NULL)
			return (1);
		if (prev_wd[ft_strlen(prev_wd) - 1] != '/')
			path = ft_strjoin(prev_wd, "/", data);
		else
			path = prev_wd;
		path2 = ft_strjoin(path, path3, data);
		free(data->pwd);
		data->pwd = ftt_strdup(path2);
		if(!data->pwd)
			free_exit(data);
		if(flag == 0)
			check_add(ft_strjoin("PWD=", path2, data), data);
		return (1);
	}
	else
	{
		if(flag == 0)
			check_add(ft_strjoin("PWD=", path, data), data);
	}
	return (0);
}

void	cd_to_home(t_data *data)
{
	char	*home;
	char	*prev_wd;

	prev_wd = data->pwd;
	home = get_home(data->env);
	if (!home)
	{
		printf("cd: HOME not set\n");
		exit_stat(1, 1);;
		return ;
	}
	if (chdir(home) == 0)
		exit_stat(update_pwd_and_oldpwd(data, NULL, prev_wd), 1);
	else
	{
		perror(home);
		exit_stat(1, 1);
	}
}

void	cd_to_path(t_data *data, char *path)
{
	char	*prev_wd;
	char	s[PATH_MAX];

	prev_wd = data->pwd;
	if (prev_wd == NULL)
		prev_wd = getcwd(s, sizeof(s));
	if (chdir(path) == 0)
		exit_stat(update_pwd_and_oldpwd(data, path, prev_wd), 1);
	else
	{
		perror(path);
		exit_stat(1, 1);
	}
}

int	prepare_cd_path(char **str, t_data *data, char **target_path)
{
	if (str[1] == NULL)
	{
		*target_path = get_home(data->env);
		if (*target_path == NULL)
		{
			write(2, "cd: HOME not set\n", 18);
			return (0);
		}
	}
	else
	{
		if (str[2] != NULL)
		{
			write(2, "cd: too many arguments\n", 24);
			return (0);
		}
		*target_path = str[1];
	}
	return (1);
}
