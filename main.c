/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 13:46:04 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/12 19:58:43 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_in_readline = 0;

void	addenv(char **env, t_data *data)
{
	int		i;

	i = 0;
	while (env[i])
	{
		check_add(env[i], data);
		i++;
	}
}

void	empty_env_init(t_data *data)
{
	char	cwd[PATH_MAX];
	char	*path;

	check_add("_=./minishell", data);
	path = getcwd(cwd, PATH_MAX);
	check_add(ft_strjoin("PWD=", path, data), data);
}

void	env_init(char **env, t_data *data)
{
	if (*env == NULL)
		empty_env_init(data);
	else
		addenv(env, data);
	if (ft_getenv2("PATH", data))
	{
		data->default_path = ftt_strdup(PATH);
		exit_stat(1, 1);
		if (data->default_path == NULL)
		{
			exit_stat(1, 1);
			free_exit(data);
		}
	}
	check_add("OLDPWD", data);
	shlvl_init(data);
}

void	data_init(t_data *data, int argc, char **argv)
{
	(void) argc;
	(void) argv;
	exit_stat(0, 1);
	data->commands = NULL;
	data->pwd = NULL;
	data->line = NULL;
	data->tokens = NULL;
	data->alloc = NULL;
	data->default_path = NULL;
	data->env = NULL;
	data->fds = NULL;
	data->fd_count = 0;
	data->token_size = 0;
	data->command_count = 0;
}

static void	process_command(t_data *data, char *cmd)
{
	if (check_quotes(cmd) != 1)
	{
		tokenize(data, cmd);
		if (lexing(data->tokens, data) == 1)
		{
			expanding(data);
			parse(data);
			if (g_in_readline != 4)
				execute(data);
			destroy_heredocs(data);
		}
	}
	if (ft_strlen(cmd) != 0)
		add_history(cmd);
}

static void	process_line(t_data *data, char **lines)
{
	char	*cmd;
	int		i;

	i = 0;
	while (lines[i])
	{
		cmd = ft_strtrim(lines[i], data);
		if (ft_strlen(cmd) == 0 && ft_strlen(lines[i]) > 0)
			add_history(lines[i]);
		if (cmd && ft_strlen(cmd) > 0)
			process_command(data, cmd);
		if (g_in_readline == 4)
			break ;
		i++;
	}
}

static void	read_command(t_data *data)
{
	char	**lines;

	if (g_in_readline == 4)
	{
		dup2(2, 0);
		exit_stat(130, 1);
	}
	g_in_readline = 1;
	data->line = readline("-> minihell ");
	if (!data->line)
		free_exit(data);
	g_in_readline = 0;
	add_data_line(data->line, data);
	lines = ft_split(data->line, '\n', data);
	process_line(data, lines);
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;

	if (!isatty(1) || !isatty(0))
		return (1);
	rl_catch_signals = 0;
	data_init(&data, argc, argv);
	env_init(env, &data);
	while (1)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, noting);
		read_command(&data);
		ft_lstclear_garbage(&data.alloc);
		data.alloc = NULL;
	}
	free_exit(&data);
	return (0);
}
