/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:19:20 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/16 12:49:54 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *build_expanded_string(char *token, char *previous, int i)
{
    char    *next;
    char    *expanded;
    int        j;
    int        k;

    j = 0;
    k = 0;
    if (token[i] == '?')
    {
        expanded = ft_strdup("0");
        j++;
    }
    else
    {
        while(token[i + j] && token[i + j] != '$')
            j++;
        expanded = malloc(sizeof(char) * (j + 1));
        ft_strlcpy(expanded, token + i, j + 1);
		if (!getenv(expanded))
			return(token);
		expanded = ft_strdup(getenv(expanded));
    }
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
	while(ft_strchr(token , '$'))
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

char	*create_spaces(int count)
{
	char *str;
	int i;

	str = malloc(sizeof(char) *(count + 1));
	i = 0;
	while(i < count)
		str[i++] = ' ';
	str[i] = '\0';
	return (str);
}

// akhra code ktbt f7yati but it works

char	*handle_quoted_token(char *token)
{
	char **strs;
	int		i;
	char	*temp;
	int j;

	j = 0;
	i = 0;	
	strs = ft_split(token, ' ');
	while(strs[i])
	{
		if (ft_strchr(strs[i], '$'))
			strs[i] = expand_token(strs[i]);
		i++;
	}
	while(token[j] && token[j] == ' ')
		j++;
	if (i == 1)
		temp = ft_strjoin(create_spaces(j) ,strs[0]);
	else
	{
		temp = ft_strjoin(create_spaces(j), strs[0]);
		i = 1;
		while(strs[i])
		{
			while(token[j] && token[j] != ' ')
				j++;
			token   = token + j;
			j = 0;
			while(token[j] && token[j] == ' ')
				j++;
			temp = ft_strjoin(temp, create_spaces(j));
			temp = ft_strjoin(temp, strs[i]);
			i++;
		}
	}
	return (temp);
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
				tokens[i]->content = handle_quoted_token(tokens[i]->content);
		}
		i++;		
	}
}
