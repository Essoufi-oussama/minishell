/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 13:43:26 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/12 14:26:51 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int 	is_special(char c)
{
	return (c == '&' || c == ';' || c == '(' || c == '`' || c == '{' || c == '}' || c == ')' || c == '*'
		|| c == '~' || c == '\\');
}

static int to_handle(char c)
{
	return(isalnum(c) || c == '|' || c == '<' || c == '>' || c == '\'' 
		|| c == '$' || c == '\"' || c == ' ' || is_special(c));
}

static	int other_ops(char c)
{
	if (c == '<')
		return INPUT_DIRECTION;
	else if (c ==  '>')
		return OUTPUT_DIRECTION;
	else if (c == '\'')
		return (SINGLE_QUOTE);
	else if (c == '$')
		return (DOLLAR);
	else if (c == '\"')
		return (DOUBLE_QUOTE);
	else if (c == ' ')
		return (WHITE_SPACE);
	else if (c == '|')
		return PIPE;
	return (SPECIAL_CHAR);
}

int	count_tokens(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (to_handle(str[i]))
		{
			count++;
			if(isalnum(str[i]))
				while(str[i] && isalnum(str[i + 1]))
					i++;
			if(str[i] == '|' && str[i + 1] == '|')
				i++;
			else if(str[i] == '>' && str[i + 1] == '>')
				i++;
			else if (str[i] == '<' && str[i + 1] == '<')
				i++;
		}
		i++ ;
	}
	return(count);
}

void	insert_token_arr(int *arr, int *i, char *str, int j)
{
	if (isalnum(str[*i]))
	{
		arr[j] = WORD;
		while(str[*i] && isalnum(str[*i + 1]))
			(*i)++;
	}
	else if (str[*i] == '|' && str[*i + 1] == '|')
	{
		arr[j] = SPECIAL_CHAR;
		(*i)++;
	}
	else if (str[*i] == '<' && str[*i + 1] == '<')
	{
		arr[j] = HERE_DOC;
		(*i)++;
	}
	else if (str[*i] == '>' && str[*i + 1] == '>')
	{
		arr[j] = OUT_APPEND;
		(*i)++;
	}
	else
		arr[j] = other_ops(str[*i]);
}

int *tokenize(char *str, int *token_size)
{
    int tokens_count;
    int i;
    int *token_arr;
    int j;

    j = 0;
    i = 0;
    tokens_count = count_tokens(str);
	*token_size = tokens_count;
    token_arr = malloc(sizeof(int) * tokens_count + 1);
    if (token_arr == NULL)
        return (NULL);
    while (str[i])
    {
        if (to_handle(str[i]))
			insert_token_arr(token_arr, &i, str, j++);
		i++;
    }
    token_arr[j] = -1;
    return (token_arr);
}
