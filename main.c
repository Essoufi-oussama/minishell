/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 13:46:04 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/19 13:08:14 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data	*env_init(char **env, int argc, char **argv)
{
	t_data	*data;

	(void)argc;
	(void)argv;
	data = malloc(sizeof(t_data));
	if(!data)
		exit(1) ;
	data->env = env;
	data->exit_status = 0;
	data->commands = NULL;
	data->line = NULL;
	data->tokens = NULL;
	return (data);
}

int	main(int argc, char **argv, char **env)
{
	
	t_data	*data;

	data = env_init(env, argc, argv);
	while(1)
	{
		data->alloc = NULL;		
		data->line = readline("-> minihell ");
		if (!data->line)
		{
			printf("exit\n");
			exit(1);
		}
		if (ft_strlen(data->line) > 0)
			add_history(data->line);
		if (check_quotes(data->line) != 1)
		{
			tokenize(data);
			if (lexing(data->tokens) == 1)
			{
				expanding(data);
				parse(data);
			}
		}
		ft_lstclear_garbage(&data->alloc);
		free(data->line);
	}
	ft_lstclear_garbage(&data->alloc);
	free(data);
	return (0);
}
