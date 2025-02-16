/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 13:46:04 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/16 13:46:07 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main()
{
	char *line;
	t_token	**tokens;
	// int token_size;

	while(1)
	{
		line = readline("-> minishell ");
		if (!line)
		{
			printf("exit\n");
			exit(1);
		}
		if (strlen(line) > 0)
			add_history(line);
		if (check_quotes(line) != 1)
		{
			tokens = tokenize(line);
			check_tokens(tokens);
			expanding(tokens);
			for(int i = 0; tokens[i]; i++)
			{
				printf("%s\n", tokens[i]->content);
				free(tokens[i]->content);
				free(tokens[i]);
			}
			free(tokens);
		}
		free(line);
	}
}