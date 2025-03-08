/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:28:30 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/08 22:58:42 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_token(char **token, int count, int *i, t_data *data)
{
	if (count > 1)
		*token = handle_multiple_dollars(*token, count, *i, data);
	else
	{
		(*i)++;
		*token = build_exp_str(*token, NULL, i, data);
	}
}

char	*expand_token(char *token, t_data *data)
{
	int		i;
	int		count;
	int		token_len;

	i = 0;
	while (token[i])
	{
		token_len = ft_strlen(token);
		if (i >= token_len)
			break ;
		if (token[i] == '$')
		{
			count = 0;
			while ((i + count) < token_len && token[i + count] == '$')
				count++;
			if (count == 1 && (i + count) >= token_len)
				break ;
			handle_token(&token, count, &i, data);
		}
		else
			i++;
	}
	return (token);
}

char	*get_expanded_value(char *token, int i, int *j, t_data *data)
{
	char	*expanded;

	*j = 0;
	if (token[i] == '?')
		return ((*j)++, ft_itoa(data->exit_status, data));
	if (!ft_isalnum(token[i]) && token[i] && token[i + *j] != '_')
		return (ft_strdup("$", data));
	else
	{
		if (ft_isdigit(token[i]))
			(*j)++;
		else
		{
			while (token[i + *j] && (token[i + *j] != '$'))
			{
				if (!ft_isalnum(token[i + *j]) && token[i + *j] != '_')
					break ;
				(*j)++;
			}
		}
	}
	expanded = ft_malloc(sizeof(char) * (*j + 1), data);
	ft_strlcpy(expanded, token + i, *j + 1);
	return (ft_getenv(expanded, data));
}

char	*handle_multiple_dollars(char *token, int count, int i, t_data *data)
{
	char	*previous;
	char	*expanded;
	int		k;
	char	*next;

	if (count % 2 != 0)
		count--;
	expanded = ft_strdup("", data);
	previous = ft_malloc(sizeof(char) * i + 1, data);
	ft_strlcpy(previous, token, i + 1);
	k = 0;
	while (token[i + count + k])
		k++;
	next = ft_malloc(sizeof(char) * (k + 1), data);
	ft_strlcpy(next, token + i + count, k + 1);
	token = ft_strjoin(previous, expanded, data);
	token = ft_strjoin(token, next, data);
	return (token);
}

char	*build_exp_str(char *token, char *previous, int *i, t_data *data)
{
	char	*next;
	char	*expanded;
	int		j;
	int		k;

	k = 0;
	expanded = get_expanded_value(token, *i, &j, data);
	previous = ft_malloc(sizeof(char) * *i, data);
	ft_strlcpy(previous, token, *i);
	while (token[*i + j + k])
		k++;
	next = ft_malloc(sizeof(char) * (k + 1), data);
	ft_strlcpy(next, token + *i + j, k + 1);
	token = ft_strjoin(previous, expanded, data);
	token = ft_strjoin(token, next, data);
	if (ft_strlen(token) == 0)
		*i = 0;
	else if (expanded && ft_strlen(expanded) == 1 && expanded[0] == '$')
		(*i)++;
	return (token);
}
