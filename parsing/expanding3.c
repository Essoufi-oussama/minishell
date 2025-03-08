/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 00:09:30 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/06 11:35:19 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_token(char *token, t_data *data)
{
	char	*previous;
	int		i;
	int		count;

	previous = NULL;
	i = 0;
	while (token[i])
	{
		if (token[i] == '$')
		{
			count = 0;
			while (token[i + count] && token[i + count] == '$')
				count++;
			if (count == 1 && token[i] && !token[i + count])
				break ;
			if (count > 1)
				token = handle_multiple_dollars(token, count, i, data);
			else
				token = build_exp_str(token, previous, i + count, data);
		}
		i++;
	}
	return (token);
}

static void	handle_single_char_token(t_token **tokens, int *i)
{
	if (tokens[*i + 1] && tokens[*i + 1]->part_of_previous)
	{
		tokens[*i + 1]->part_of_previous = 0;
		remove_token(tokens, *i);
		if (*i > 0)
			(*i)--;
	}
	else
		(*i)++;
}

static void	handle_empty_token(t_token **tokens, int *i, char *original)
{
	if (tokens[*i + 1] && tokens[*i + 1]->part_of_previous)
	{
		tokens[*i + 1]->part_of_previous = 0;
		remove_token(tokens, *i);
		if (*i > 0)
			(*i)--;
	}
	else
		tokens[(*i)++]->content = original;
}

void	handle_redirect(t_token **tokens, int *i, t_data *data)
{
	char	*original;

	if (!tokens[*i])
		return ;
	if (ft_strlen(tokens[*i]->content) == 1)
	{
		handle_single_char_token(tokens, i);
		return ;
	}
	original = ft_strdup(tokens[*i]->content, data);
	if (ft_strchr(tokens[*i]->content, '$'))
		tokens[*i]->content = expand_token(tokens[*i]->content, data);
	if (ft_strlen(tokens[*i]->content) == 0)
		handle_empty_token(tokens, i, original);
	else
		(*i)++;
}
