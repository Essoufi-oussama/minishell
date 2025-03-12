/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 13:46:04 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/12 08:12:40 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_in_readline = 0;

t_data	*env_init(char **env)
{
	t_data	*data;
	t_env	*env_list;

	data = malloc(sizeof(t_data));
	if (!data)
		exit(1);
	env_list = NULL;
	addenv(env, &env_list);
	exit_stat(0, 1);
	data->commands = NULL;
	data->pwd = NULL;
	data->line = NULL;
	data->tokens = NULL;
	data->alloc = NULL;
	data->default_path = NULL;
	if (*env == NULL)
	{
		data->env = NULL;
		data->default_path = ftt_strdup(PATH);
		if (data->default_path == NULL)
			free_exit(data);
	}
	else
		data->env = env_list;
	return (data);
}

int exit_stat(int status, int flag)
{
	static int s = 0;

	if (flag != 0)
		s = status;
	return (s);
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

	data->alloc = NULL;
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
	t_data	*data;

	if(!isatty(0) || !isatty(1) || !isatty(2))
		return(1);
	(void)argc;
	(void)argv;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, noting);
	data = env_init(env);
	while (1)
	{
		rl_catch_signals = 0;
		read_command(data);
		ft_lstclear_garbage(&data->alloc);
	}
	free_exit(data);
	return (0);
}
