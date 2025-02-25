/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 13:46:04 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/25 14:06:19 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstclear_env(t_env **lst)
{
	t_env	*tmp;

	if (lst == NULL)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free((*lst)->env_var);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

void	free_exit(t_data *data)
{
	ft_lstclear_garbage(&data->alloc);
	ft_lstclear_env(&data->env);
	free(data);
}

t_data	*env_init(char **env, int argc, char **argv)
{
	t_data	*data;
	t_env *env_list;

	(void)argc;
	(void)argv;
	data = malloc(sizeof(t_data));
	if(!data)
		exit(1) ;
	env_list = NULL;
    addenv(env, &env_list);
	data->exit_status = 0;
	data->commands = NULL;
	data->line = NULL;
	data->tokens = NULL;
	data->env = env_list;
	return (data);
}

void	tokenize_new_stuff(char *cmd, t_data *data)
{
	data->pipe_line = cmd;
	tokenize_pipe(data);
}

static void handle_pipe_at_end(t_data *data, char **cmd)
{
    char *line;
    char *tmp_cmd;
    char *space_joined;
    char *final_joined;

    if (check_quotes(*cmd, data) != 1)
    {
        tokenize(data);
        expanding(data);
        parse(data);
    }
    else
        return;
    while (*cmd && (*cmd)[ft_strlen(*cmd) - 1] == '|')
    {
        line = readline("> ");
        if (!line)
        {
            printf("exit\n");
            exit(1);
        }
        char *trimmed_line = ft_strtrim(line, data);
		tmp_cmd = ft_strdup(*cmd, data);
		space_joined = ft_strjoin(tmp_cmd, " ", data);
		final_joined = ft_strjoin(space_joined, trimmed_line, data);
		*cmd = ft_strtrim(final_joined, data);
        if (ft_strlen(trimmed_line) == 0)
            continue;
        if (ft_strlen(trimmed_line) != 1 && trimmed_line[0] != '|' && trimmed_line[ft_strlen(trimmed_line) - 1] == '|')
        {
            tokenize_new_stuff(trimmed_line, data);
			if (lexing_new_stuff(data->readline_tokens, data) != 1)
				break;
            expanding_new_stuff(data);
            parsing_new_stuff(data);
        }
        else
        {
            if (check_quotes(trimmed_line, data) != 1)
            {
                tokenize_new_stuff(trimmed_line, data);
                if (lexing_new_stuff(data->readline_tokens, data) == 1)
                {
                    expanding_new_stuff(data);
                    parsing_new_stuff(data);
                    execute(data);
                }
            }
            break;
        }
		free(line);
    }
	if (ft_strlen(*cmd) != 0)
		add_history(*cmd);
}


static void	process_command(t_data *data, char *cmd)
{
	if (check_quotes(cmd, data) != 1)
	{
		data->line = cmd;
		tokenize(data);
		if (lexing(data->tokens, data) == 1)
		{
			
			expanding(data);
			parse(data);
			execute(data);
		}
	}
	if (ft_strlen(cmd) != 0)
		add_history(cmd);
}

static void	handle_command(t_data *data, char *cmd)
{
	if (cmd && ft_strlen(cmd) > 1 && cmd[ft_strlen(cmd) - 1] == '|')
		handle_pipe_at_end(data, &cmd);
	else
		process_command(data, cmd);
	
}

static void	read_command(t_data *data)
{
	char	*cmd;
	char	*tmp;

	data->alloc = NULL;
	data->line = readline("-> minihell ");
	if (!data->line)
	{
		printf("exit\n");
		exit(1);
	}
	tmp = data->line;
	cmd = ft_strtrim(data->line, data);
	if (ft_strlen(cmd) == 0 && ft_strlen(data->line) > 0)
		add_history(data->line);
	handle_command(data, cmd);
	free(tmp);
}

int	main(int argc, char **argv, char **env)
{
	t_data	*data;

	data = env_init(env, argc, argv);
	while (1)
	{
		read_command(data);
		ft_lstclear_garbage(&data->alloc);
	}
	free_exit(data);
	return (0);
}