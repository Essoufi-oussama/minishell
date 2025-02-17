/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 13:46:04 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/16 19:36:39 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main()
{
	char *line;
	t_token	**tokens;
	t_alloc *garbage;

	while(1)
	{
		garbage = NULL;
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
			tokens = tokenize(line, &garbage);
			check_tokens(tokens);
			expanding(tokens, &garbage);
			for(int i = 0; tokens[i]; i++)
			{
				if (tokens[i]->part_of_previous)
					printf("part of previous ");
				printf("%s\n", tokens[i]->content);
			}
		}
		ft_lstclear_garbage(&garbage);
	}

}