/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 13:38:17 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/16 18:19:54 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*create_spaces(int count, t_alloc **head)
{
	char	*str;
	int		i;

	str = ft_malloc(sizeof(char) * (count + 1), head);
	i = 0;
	while (i < count)
		str[i++] = ' ';
	str[i] = '\0';
	return (str);
}

char	*join_tokens(char **strs, char *token, int j, t_alloc **head)
{
	char	*temp;
	int		i;

	i = 1;
	temp = ft_strjoin(create_spaces(j, head), strs[0], head);
	while (strs[i])
	{
		while (token[j] && token[j] != ' ')
			j++;
		token = token + j;
		j = 0;
		while (token[j] && token[j] == ' ')
			j++;
		temp = ft_strjoin(temp, create_spaces(j, head), head);
		temp = ft_strjoin(temp, strs[i], head);
		i++;
	}
	return (temp);
}

char	*handle_quoted_token(char *token, t_alloc **head)
{
	char	**strs;
	int		i;
	int		j;

	j = 0;
	i = 0;
	strs = ft_split(token, ' ', head);
	while (strs[i])
	{
		if (ft_strchr(strs[i], '$'))
			strs[i] = expand_token(strs[i], head);
		i++;
	}
	while (token[j] && token[j] == ' ')
		j++;
	if (i == 1)
		return (ft_strjoin(create_spaces(j, head), strs[0], head));
	return (join_tokens(strs, token, j, head));
}
