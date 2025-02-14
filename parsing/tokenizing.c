/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:33:05 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/14 20:58:21 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*insert_token_arr_word(int *i, char *str, int quote)
{
	char *new;
	int j;
	t_token *token;

	j = 0;
	token = malloc(sizeof(t_token));
	if (quote == S_QUOTE) 
        while (str[j] && str[j] != '\'')
            j++;
    else if (quote == D_QUOTE) 
        while (str[j] && str[j] != '\"')
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
	token->quoted = (quote != NO_QUOTE);
	token->type = WORD;
	*i += j;
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
		token->type = SPECIAL_CHAR;
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
	*i += 1;
	return (token);
}

void	loop_token_arr(char *str, t_token **token_arr)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (str[i])
    {
        if (str[i] == '\'' || str[i] =='\"') 
		{
			if (!str[++i])
				break;
			if (str[i - 1] == '\'')
            	token_arr[j++] = insert_token_arr_word(&i, str + i, S_QUOTE);
			else if (str[i - 1] == '\"')
				token_arr[j++] = insert_token_arr_word(&i, str + i, D_QUOTE);
			i++;
        } 
		else if (is_word_char(str[i])) 
            token_arr[j++] = insert_token_arr_word(&i, str + i, NO_QUOTE);
		else if (to_handle(str[i])) 
            token_arr[j++] = insert_token_arr_op(&i, str + i);
        else 
            i++;
    }
    token_arr[j] = NULL;
}

t_token **tokenize(char *str)
{
    int tokens_count;
    int i;
    int j;
    t_token **token_arr;

	i = 0;
	j = 0;
    tokens_count = count_tokens(str);
    token_arr = malloc(sizeof(t_token *) * (tokens_count + 1));
    loop_token_arr(str, token_arr);
    return (token_arr);
}
