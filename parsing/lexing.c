/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:40:59 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/18 14:46:29 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_quotes(char *str)
{
	int		i;
	char	quote;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				i++;
			if (!str[i])
			{
				printf("Syntax error unclosed quote\n");
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	is_operation(int i)
{
	return (i == INPUT_DIRECTION || i == OUTPUT_DIRECTION
		|| i == OUT_APPEND || i == HERE_DOC || i == PIPE);
}

int	check_consecutive_expressions(t_token **tokens)
{
	int		i;
	t_token	*previous;

	i = 0;
	if (tokens[0]->type == INPUT_DIRECTION)
		if (check_end_and_pipe(tokens) == 1)
			return (1);
	previous = tokens[i++];
	while (tokens[i])
	{
		if (is_operation(previous->type) && is_operation(tokens[i]->type))
		{
			printf("syntax error near unexpected token '%s'\n",
				tokens[i]->content);
			return (1);
		}
		previous = tokens[i];
		i++;
	}
	return (0);
}

int	check_end_and_pipe(t_token **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		i++;
	if (tokens[0]->type == PIPE)
	{
		printf("synatx error near unexpected token '|'\n");
		return (1);
	}
	if (is_operation(tokens[i - 1]->type))
	{
		printf("synatx error near unexpected token 'newline'\n");
		return (1);
	}
	return (0);
}

void	lexing(t_token **tokens)
{
	int	i;

	i = 0;
	if (tokens[i] == NULL)
		return ;
	if (check_consecutive_expressions(tokens) == 1)
		return ;
	if (check_end_and_pipe(tokens) == 1)
		return ;
}
