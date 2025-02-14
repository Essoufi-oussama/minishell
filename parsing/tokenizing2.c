/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:33:05 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/14 16:42:12 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int is_word_char(char c)
{
    return (isalnum(c) || c == '_' || c == '.' || c == '/' || c == '-' || c == '$');
}

int 	is_special(char c)
{
	return (c == '&' || c == ';' || c == '(' || c == '`' || c == '{' || c == '}' || c == ')' || c == '*'
		|| c == '~' || c == '\\');
}

int to_handle(char c)
{
	return(c == '|' || c == '<' || c == '>' || is_special(c));
}

int	count_tokens(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			count++;
			i++;
			while(str[i] && str[i] != '\'')
				i++;
		}
		if (str[i] == '\"')
		{
			count++;
			i++;
			while(str[i] && str[i] != '\"')
				i++;
		}
		if (is_word_char(str[i]) || to_handle(str[i]))
		{
			count++;
			if(is_word_char(str[i]))
				while(str[i] && is_word_char(str[i + 1]))
					i++;
			if(str[i] == '>' && str[i + 1] == '>')
				i++;
			else if (str[i] == '<' && str[i + 1] == '<')
				i++;
		}
		i++ ;
	}
	return(count);
}

int count_no_quotes(char *str)
{
	int i;

	i = 0;
	while (str[i] && str[i] != ' ' && is_word_char(str[i]))
		i++ ;
	return (i);
}

t_token	*insert_token_arr_word(int *i, char *str, int quote)
{
	char *new;
	int j;
	t_token *token;

	j = 0;
	token = malloc(sizeof(t_token));
	if (quote == S_QUOTE || quote == D_QUOTE)
		while(str[j] && (str[j] != '\'' && str[j] != '\"'))
			j++;
	else
		j = count_no_quotes(str);
	new = malloc(sizeof(char) * (j + 1));
	ft_strlcpy(new, str, j + 1);
	if ((quote ==  D_QUOTE || quote == NO_QUOTE) && ft_strrchr(new, '$'))
		token->expandable = 1;
	else
		token->expandable = 0;
	token->content = new;
	if (quote == NO_QUOTE)
		token->quoted = 0;
	else
		token->quoted = 1;
	token->type = WORD;
	*i = *i + j;
	return(token);
}

void	insert_other_ops(t_token *token, char *str)
{
	char	*new;

	if (str[0] == '|')
		token->type = PIPE;
	else if (str[0] == '<')
		token->type = INPUT_DIRECTION;
	else if (str[0] == '>')
		token->type = OUTPUT_DIRECTION;
	else
		token->type = SPECIAL_CHAR;
	new = malloc(sizeof(char) * 2);
	ft_strlcpy(new, str, 2);
	token->content = new;
}

t_token	*insert_token_arr_op(int *i, char *str)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (str[0] == '<' && str[1] == '<')
	{
		token->type = HERE_DOC;
		token->content = ft_strdup("<<");
		*i += 1;
	}
	else if (str[0] == '|' && str[1] == '|')
	{
		token->type = HERE_DOC;
		token->content = ft_strdup("||");
		*i += 1;
	}
	else if (str[0] == '>' && str[1] == '>')
	{
		token->type = OUT_APPEND;
		token->content = ft_strdup(">>");
		*i += 1;
	}
	else
		insert_other_ops(token, str);
	token->quoted = 0;
	token->expandable = 0;
	return (token);
}

t_token **tokenize(char *str)
{
    int tokens_count;
    int i;
    t_token **token_arr;
    int j;

    j = 0;
    i = 0;
    tokens_count = count_tokens(str);
    token_arr = malloc(sizeof(t_token *) * (tokens_count + 1));
    while (str[i])
    {
		if (str[i] == '\'')
			token_arr[j++] = insert_token_arr_word(&i, str + ++i, S_QUOTE);
		else if (str[i] == '\"')
			token_arr[j++] = insert_token_arr_word(&i, str + ++i, D_QUOTE);
		else if (is_word_char(str[i]))
			token_arr[j++] = insert_token_arr_word(&i, str + i, NO_QUOTE);
		else if (to_handle(str[i]))
			token_arr[j++] = insert_token_arr_op(&i, str + i);
		i++;
    }
    token_arr[j] = NULL;
    return (token_arr);
}
