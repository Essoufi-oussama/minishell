/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:19:20 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/16 13:45:39 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_expanded_value(char *token, int i, int *j)
{
	char	*expanded;

	*j = 0;
	if (token[i] == '?')
	{
		expanded = ft_strdup("0");
		(*j)++;
		return (expanded);
	}
	while (token[i + *j] && token[i + *j] != '$')
		(*j)++;
	expanded = malloc(sizeof(char) * (*j + 1));
	ft_strlcpy(expanded, token + i, *j + 1);
	if (!getenv(expanded))
		return (NULL);
	return (ft_strdup(getenv(expanded)));
}

char	*build_expanded_string(char *token, char *previous, int i)
{
	char	*next;
	char	*expanded;
	int		j;
	int		k;

	k = 0;
	expanded = get_expanded_value(token, i, &j);
	if (!expanded)
		return (token);
	previous = malloc(sizeof(char) * i);
	ft_strlcpy(previous, token, i);
	while (token[i + j + k])
		k++;
	next = malloc(sizeof(char) * (k + 1));
	ft_strlcpy(next, token + i + j, k + 1);
	token = ft_strjoin(previous, expanded);
	token = ft_strjoin(token, next);
	return (token);
}

char	*expand_token(char *token)
{
	char	*previous;
	int		i;

	i = 0;
	previous = NULL;
	while (ft_strchr(token, '$'))
	{
		while (token[i] && token[i] != '$')
			i++;
		while (token[i] && token[i] == '$')
			i++;
		if (!token[i])
			break ;
		token = build_expanded_string(token, previous, i);
	}
	return (token);
}

void	expanding(t_token **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->expandable)
		{
			if (tokens[i]->quoted != D_QUOTE)
				tokens[i]->content = expand_token(tokens[i]->content);
			else
				tokens[i]->content = handle_quoted_token(tokens[i]->content);
		}
		i++;
	}
}
