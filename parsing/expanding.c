/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:19:20 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/18 14:01:25 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_expanded_value(char *token, int i, int *j, t_data *data)
{
	char	*expanded;

	*j = 0;
	if (token[i] == '?')
	{
		(*j)++;
		return (ft_itoa(data->exit_status, data));
	}
	while (token[i + *j] && (token[i + *j] != '$'))
	{
		if(token[i + *j] == '\'' || token[i + *j] == '\"')
			break ;
		(*j)++;
	}
	expanded = ft_malloc(sizeof(char) * (*j + 1), data);
	ft_strlcpy(expanded, token + i, *j + 1);
	if (!getenv(expanded))
		return (ft_strdup("", data));
	return (ft_strdup(getenv(expanded), data));
}

char	*build_exp_str(char *token, char *previous, int i, t_data *data)
{
	char	*next;
	char	*expanded;
	int		j;
	int		k;

	k = 0;
	expanded = get_expanded_value(token, i, &j, data);
	previous = ft_malloc(sizeof(char) * i, data);
	ft_strlcpy(previous, token, i);
	while (token[i + j + k])
		k++;
	next = ft_malloc(sizeof(char) * (k + 1), data);
	ft_strlcpy(next, token + i + j, k + 1);
	token = ft_strjoin(previous, expanded, data);
	token = ft_strjoin(token, next, data);
	return (token);
}

char	*handle_multiple_dollars(char	*token, int count, int i, t_data *data)
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
	while(token[i + count + k])
		k++;
	next = ft_malloc(sizeof(char)* (k + 1), data);
	ft_strlcpy(next, token + i + count, k + 1);
	token = ft_strjoin(previous, expanded, data);
	token = ft_strjoin(token, next, data);
	return (token);
}


char	*expand_token(char *token, t_data *data)
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
			token = handle_multiple_dollars(token, count, i, data);
		else
			token = build_exp_str(token, previous, i + count, data);
	}
	return (token);
}

int	skip_here_doc_limiter(t_token **tokens, int current)
{
	while(current >= 0 && tokens[current]->part_of_previous)
		current--;
	if (current > 0)
	{
		if(tokens[current - 1]->type == HERE_DOC)
			return (0);
	}
	return (1);
}

void	expanding(t_data *data)
{
	int	i;
	t_token **tokens;

	tokens = data->tokens;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->expandable && skip_here_doc_limiter(tokens, i))
		{
			if (tokens[i]->quoted != D_QUOTE)
				tokens[i]->content = expand_token(tokens[i]->content, data);
			else
				tokens[i]->content = handle_quoted_token(tokens[i]->content, data);
		}
		i++;
	}
}
