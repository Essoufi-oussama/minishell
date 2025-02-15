/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:19:20 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/15 21:10:39 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_expanded_string(char *token, char *previous, int i)
{
	char	*next;
	char	*expanded;
	int		j;
	int		k;

	j = 0;
	k = 0;
	while(token[i + j] && token[i + j] != '$')
		j++;
	expanded = malloc(sizeof(char) * (j + 1));
	ft_strlcpy(expanded, token + i, j + 1);
	if (getenv(expanded))
		expanded = ft_strdup(getenv(expanded));
	else
		i++;
	previous = malloc(sizeof(char) * (i));
	ft_strlcpy(previous, token, i);
	while(token[i + j + k])
		k++;
	next = malloc(sizeof(char) *(k + 1));
	ft_strlcpy(next, token + i + j, k + 1);
	token = ft_strjoin(previous, expanded);
	token = ft_strjoin(token, next);
	return(token);
}

char	*expand_token(char *token)
{
	char	*previous;
	
	int		i;
	i = 0;
	previous = NULL;
	while(ft_strchr(token, '$'))
	{
		while (token[i] && token[i] != '$')
			i++;
		while(token[i] && token[i] == '$')
			i++;
		if (!token[i])
			break ;
		token = build_expanded_string(token, previous, i);	
	}
	return (token);
}

void	handle_quoted_token(t_token *token)
{
	char **strs;
	int		i;

	i = 0;	
	strs = ft_split(token->content, ' ');
	while(strs[i])
	{
		if (ft_strchr(strs[i], '$'))
			strs[i] = expand_token(strs[i]);
		i++;
	}
	if (i == 1)
		token->content = strs[0];
	else
	{
		token->content = strs[0];
		i = 1;
		while(strs[i])
		{
			token->content = ft_strjoin(token->content, " ");
			token->content = ft_strjoin(token->content, strs[i]);
			i++;
		}
	}
}

void	expanding(t_token **tokens)
{
	int	i;
	i = 0;

	while(tokens[i])
	{
		if (tokens[i]->expandable)
		{
			if (tokens[i]->quoted != D_QUOTE)
				tokens[i]->content = expand_token(tokens[i]->content);
			else
				handle_quoted_token(tokens[i]);
		}
		i++;		
	}
}
