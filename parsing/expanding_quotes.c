/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 13:38:17 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/16 13:59:22 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_spaces(int count)
{
	char	*str;
	int		i;

	str = malloc(sizeof(char) *(count + 1));
	i = 0;
	while (i < count)
		str[i++] = ' ';
	str[i] = '\0';
	return (str);
}

char	*join_tokens(char **strs, char *token, int j)
{
	char	*temp;
	int		i;

	i = 1;
	temp = ft_strjoin(create_spaces(j), strs[0]);
	while (strs[i])
	{
		while (token[j] && token[j] != ' ')
			j++;
		token = token + j;
		j = 0;
		while (token[j] && token[j] == ' ')
			j++;
		temp = ft_strjoin(temp, create_spaces(j));
		temp = ft_strjoin(temp, strs[i]);
		i++;
	}
	return (temp);
}

char	*handle_quoted_token(char *token)
{
	char	**strs;
	int		i;
	int		j;

	j = 0;
	i = 0;
	strs = ft_split(token, ' ');
	while (strs[i])
	{
		if (ft_strchr(strs[i], '$'))
			strs[i] = expand_token(strs[i]);
		i++;
	}
	while (token[j] && token[j] == ' ')
		j++;
	if (i == 1)
		return (ft_strjoin(create_spaces(j), strs[0]));
	return (join_tokens(strs, token, j));
}
