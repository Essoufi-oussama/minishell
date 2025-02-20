/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 13:46:04 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/20 20:12:33 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_init(char **env, t_data *data)
{
	int	i;

	i = 0;
	if (env == NULL)
	{
		data->env = NULL;
	}
	while (env[i])
		i++;
	data->env = ft_malloc(sizeof(char *) * (i + 1), data);
	i = 0;
	while(env[i])
	{
		data->env[i] = ft_strdup(env[i], data);
		i++;
	}
	data->env[i] = NULL;
}

t_data	*data_init(int argc, char **argv, char **env)
{
	t_data	*data;

	(void)argc;
	(void)argv;
	data = malloc(sizeof(t_data));
	if(!data)
		exit(1) ;
	data->exit_status = 0;
	data->commands = NULL;
	data->line = NULL;
	data->tokens = NULL;
	data->alloc = NULL;
	env_init(env, data);
	return (data);
}

int	main(int argc, char **argv, char **env)
{
	
	t_data	*data;

	data = data_init(argc, argv, env);
	while(1)
	{
		data->line = readline("-> minihell ");
		if (!data->line)
		{
			printf("exit\n");
			exit(1);
		}
		if (ft_strlen(data->line) > 0)
			add_history(data->line);
		if (check_quotes(data->line, data) != 1)
		{
			tokenize(data);
			if (lexing(data->tokens, data) == 1)
			{
				expanding(data);
				parse(data);
			}
		}
		// ft_lstclear_garbage(&data->alloc);
		// data->alloc = NULL;		
		free(data->line);
	}
	ft_lstclear_garbage(&data->alloc);
	free(data);
	return (0);
}
