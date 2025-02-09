/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_test.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 13:43:26 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/09 17:02:07 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_tokens(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if(isalnum(str[i]))
		{
			count++;
			while(str[i] && isalnum(str[i]))
				i++;
		}
		if (str[i] == '|')
			count++;
		else if (str[i] == '<')
		{
			count++;
			if (str[i + 1] == '<')
				i++;
		}
		else if (str[i] == '>')
		{
			count++;
			if (str[i + 1] == '>')
				i++;
		}
		else if (str[i] == '\'')
			count++;
		else if (str[i] == '$')
			count++;
		else if (str[i] == '\"')
			count++;
		i++;
	}
	return(count);
}

int	*tokenize(char *line)
{
	int	tokens_count;
	int	i;
	int	*token_arr;
	int	j;

	j = 0;
	i = 0;
	tokens_count = count_tokens(line);
	token_arr = malloc(sizeof(int) * tokens_count + 1);
	if(token_arr == NULL)
		return (NULL);
	while (line[j])
	{
		if(isalnum(line[j]))
		{
			token_arr[i++] = WORD;
			while(line[j] && isalnum(line[j]))
				j++;
		}
		else if (line[j] == '|')
			token_arr[i++] = PIPE;
		else if (line[j] == '<')
		{
			if (line [j + 1] == '<')
			{
				token_arr[i++] = HERE_DOC;
				j++;
			}
			else
				token_arr[i++] = INPUT_DIRECTION;
		}
		else if (line[j] == '>')
		{
			if (line [j + 1] == '>')
			{
				token_arr[i++] = OUT_APPEND;
				j++;
			}
			else
				token_arr[i++] = OUTPUT_DIRECTION;
		}
		else if (line[j] == '\'')
			token_arr[i++] = SINGLE_QUOTE;
		else if (line[j] == '$')
			token_arr[j++] = DOLLAR;
		else if (line[j] == '\"')
			token_arr[i++] = DOUBLE_QUOTE;
		j++;
	}
	token_arr[i] = -1;
	return (token_arr);
}