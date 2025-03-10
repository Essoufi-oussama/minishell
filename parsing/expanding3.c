/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 00:09:30 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/10 16:58:57 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9'))
		return (c);
	return (0);
}

int	ft_strcmp_env(const char *s1, const char *s2)
{
	size_t	i;
	size_t	n;
	
	n = 0;
	while (s1[n] && s1[n] != '=')
		n++;
	i = 0;
	while (i < n && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if (s2[i])
		return (-1);
	return (0);
}


char	*ft_getenv(char *str, t_data *data)
{
	t_env	*current;
	char	*value;

	if (data->env == NULL && ft_strcmp(str, "PATH") == 0)
		return (ft_strdup(data->default_path, data));
	current = data->env;
	while (current)
	{
		if (ft_strcmp_env(current->env_var, str) == 0)
		{
			value = ft_strchr(current->env_var, '=');
			if (value == NULL)
				return (ft_strdup("", data));
			else
				return (ft_strdup(value + 1, data));
		}
		current = current->next;
	}
	return (ft_strdup("", data));
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
	{
		tokens[*i]->ambigious = 1;
		tokens[(*i)++]->content = original;
	}
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
	{
		if (ft_strchr(tokens[*i]->content, ' '))
		{
			tokens[*i]->content = original;
			tokens[*i]->ambigious = 1;
		}
		(*i)++;
	}
}
