/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   counting_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:23:14 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/16 13:58:24 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_word_char(char c)
{
	return (!to_handle(c) && ft_isprint(c) && c != '\'' && c != '\"');
}

int	to_handle(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	handle_quotes(char *str, int *i, char quote_char)
{
	int	count;

	count = 0;
	if (str[*i] == quote_char)
	{
		count++;
		(*i)++;
		while (str[*i] && str[*i] != quote_char)
			(*i)++;
		if (str[*i] == quote_char)
			(*i)++;
	}
	return (count);
}

int	count_tokens(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		count += handle_quotes(str, &i, '\'');
		count += handle_quotes(str, &i, '\"');
		if (is_word_char(str[i]) || to_handle(str[i]))
		{
			count++;
			if (is_word_char(str[i]))
				while (str[i] && is_word_char(str[i + 1]))
					i++;
			if (str[i] == '>' && str[i + 1] == '>')
				i++;
			else if (str[i] == '<' && str[i + 1] == '<')
				i++;
		}
		if (str[i])
			i++ ;
	}
	return (count);
}

int	count_no_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != ' ' && is_word_char(str[i]))
		i++ ;
	return (i);
}
