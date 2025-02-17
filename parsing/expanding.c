/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:19:20 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/17 20:55:20 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_expanded_value(char *token, int i, int *j, t_alloc **head)
{
	char	*expanded;

	*j = 0;
	if (token[i] == '?')
	{
		expanded = ft_strdup("0", head);
		(*j)++;
		return (expanded);
	}
	while (token[i + *j] && (token[i + *j] != '$'))
	{
		if(token[i + *j] == '\'' || token[i + *j] == '\"')
			break ;
		(*j)++;
	}
	expanded = ft_malloc(sizeof(char) * (*j + 1), head);
	ft_strlcpy(expanded, token + i, *j + 1);
	if (!getenv(expanded))
		return (ft_strdup("", head));
	return (ft_strdup(getenv(expanded), head));
}

char	*build_exp_str(char *token, char *previous, int i, t_alloc **head)
{
	char	*next;
	char	*expanded;
	int		j;
	int		k;

	k = 0;
	expanded = get_expanded_value(token, i, &j, head);
	previous = ft_malloc(sizeof(char) * i, head);
	ft_strlcpy(previous, token, i);
	while (token[i + j + k])
		k++;
	next = ft_malloc(sizeof(char) * (k + 1), head);
	ft_strlcpy(next, token + i + j, k + 1);
	token = ft_strjoin(previous, expanded, head);
	token = ft_strjoin(token, next, head);
	return (token);
}

char	*handle_multiple_dollars(char	*token, int count, int i, t_alloc **head)
{
	char	*previous;
	char	*expanded;
	int		k;
	char	*next;

	if (count % 2 != 0)
		count--;
	expanded = ft_strdup("", head);
	previous = ft_malloc(sizeof(char) * i + 1, head);
	ft_strlcpy(previous, token, i + 1);
	k = 0;
	while(token[i + count + k])
		k++;
	next = ft_malloc(sizeof(char)* (k + 1), head);
	ft_strlcpy(next, token + i + count, k + 1);
	token = ft_strjoin(previous, expanded, head);
	token = ft_strjoin(token, next, head);
	return (token);
}

char	*expand_token(char *token, t_alloc **head)
{
	char	*previous;
	int		i;
	int		count;

	previous = NULL;
	while (ft_strchr(token, '$'))
	{
		count = 0;
		i = 0;
		while (token[i] && token[i] != '$')
			i++;
		while (token[count + i] && token[count + i] == '$')
			count++;
		if (count == 1 && token[i] && !token[i + count])
			break;
		if (count > 1)
			token = handle_multiple_dollars(token, count, i, head);
		else
			token = build_exp_str(token, previous, i + count, head);
	}
	return (token);
}

void	expanding(t_token **tokens, t_alloc **head)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->expandable)
		{
			if (tokens[i]->quoted != D_QUOTE)
				tokens[i]->content = expand_token(tokens[i]->content, head);
			else
				tokens[i]->content = handle_quoted_token(tokens[i]->content, head);
		}
		i++;
	}
}
