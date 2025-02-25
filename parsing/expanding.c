/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:19:20 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/25 13:04:32 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
		(c >= '0' && c <= '9'))
		return (c);
	return (0);
}

char	*ft_getenv(char *str, t_data *data)
{
	t_env *current;

	if (data->env == NULL)
		return (ft_strdup("", data));
	current = data->env;
	while(current)
	{
		if (ft_strncmp(current->env_var, str, ft_strlen(str)) == 0)
			return(ft_strdup(ft_strchr(current->env_var, '=') + 1, data));
		current = current->next;
	}
	return(ft_strdup("", data));
}

char	*get_expanded_value(char *token, int i, int *j, t_data *data)
{
	char	*expanded;

	*j = 0;
	if (token[i] == '?')
	{
		(*j)++;
		return (ft_itoa(data->exit_status, data));
	}
	if(!ft_isalnum(token[i]) && token[i] && token[i + *j] != '_')
	{
		while (token[i + *j] && (token[i + *j] != '$'))
			(*j)++;
	}
	else
	{
		while (token[i + *j] && (token[i + *j] != '$'))
		{
			if(!ft_isalnum(token[i + *j]) && token[i + *j] != '_')
				break ;
			(*j)++;
		}
	}
	expanded = ft_malloc(sizeof(char) * (*j + 1), data);
	ft_strlcpy(expanded, token + i, *j + 1);
	return (ft_getenv(expanded, data));
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

int	skip_ops_except_pipe(t_token **tokens, int current)
{
	while(current >= 0 && tokens[current]->part_of_previous)
		current--;
	if (current > 0)
	{
		if(is_operation(tokens[current - 1]->type) && tokens[current - 1]->type != PIPE)
			return (0);
	}
	return (1);
}

void	remove_token(t_token **tokens, int index)
{
	int	i;

	i = index;
	while (tokens[i])
	{
		tokens[i] = tokens[i + 1];
		i++;
	}
}

void	expanding(t_data *data)
{
	int	i;
	t_token **tokens;

	tokens = data->tokens;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->expandable && skip_ops_except_pipe(tokens, i))
		{
			if (tokens[i]->quoted == D_QUOTE)
			{
				tokens[i]->split_later = 0;
				tokens[i]->content = expand_token(tokens[i]->content, data);
			}
			else if (tokens[i]->quoted == NO_QUOTE)
			{
				tokens[i]->split_later = 1;
				if (ft_strlen(tokens[i]->content) == 1 && tokens[i + 1] && tokens[i + 1]->part_of_previous)
				{
					tokens[i + 1]->part_of_previous = 0;
					remove_token(tokens, i);
					if (i > 0)
						i--;
					continue;
				}
				else
				{
					tokens[i]->content = expand_token(tokens[i]->content, data);
					if (ft_strlen(tokens[i]->content) == 0)
					{
						remove_token(tokens, i);
						if (i > 0)
							i--;
						continue;
					}
				}
			}
			else
				tokens[i]->content = handle_quoted_token(tokens[i]->content, data);
		}
		i++;
	}
}

void	expanding_new_stuff(t_data *data)
{
	int	i;
	t_token **tokens;

	tokens = data->readline_tokens;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->expandable && skip_ops_except_pipe(tokens, i))
		{
			if (tokens[i]->quoted == D_QUOTE)
			{
				tokens[i]->split_later = 0;
				tokens[i]->content = expand_token(tokens[i]->content, data);
			}
			else if (tokens[i]->quoted == NO_QUOTE)
			{
				tokens[i]->split_later = 1;
				if (ft_strlen(tokens[i]->content) == 1 && tokens[i + 1] && tokens[i + 1]->part_of_previous)
				{
					tokens[i + 1]->part_of_previous = 0;
					remove_token(tokens, i);
					if (i > 0)
						i--;
					continue;
				}
				else
				{
					tokens[i]->content = expand_token(tokens[i]->content, data);
					if (ft_strlen(tokens[i]->content) == 0)
					{
						remove_token(tokens, i);
						if (i > 0)
							i--;
						continue;
					}
				}
			}
			else
				tokens[i]->content = handle_quoted_token(tokens[i]->content, data);
		}
		i++;
	}
}
