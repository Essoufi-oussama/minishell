/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 13:46:04 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/18 13:25:22 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data	*env_init(char **env)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if(!data)
		exit(1) ;
	data->env = env;
	data->exit_status = 0;
	data->comands = NULL;
	data->line = NULL;
	data->tokens = NULL;
	return (data);
}

int	main(int argc, char **argv, char **env)
{
	
	t_data	*data;

	data = env_init(env);
	argc = 0;
	argv = NULL;
	while(1)
	{
		data->alloc = NULL;		
		data->line = readline("-> minishell ");
		if (!data->line)
		{
			printf("exit\n");
			exit(1);
		}
		if (strlen(data->line) > 0)
			add_history(data->line);
		if (check_quotes(data->line) != 1)
		{
			tokenize(data);
			check_tokens(data->tokens);
			expanding(data);
			for(int i = 0; data->tokens[i]; i++)
			{
				// if (tokens[i]->part_of_previous)
				// 	printf("part of previous ");
				printf("%s\n", data->tokens[i]->content);
			}
		}
		ft_lstclear_garbage(&data->alloc);
	}

}