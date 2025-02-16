/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:33:05 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/16 18:19:34 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*insert_token_arr_word(int *i, char *str, int quote, t_alloc **head)
{
	char	*new;
	int		j;
	t_token	*token;

	j = 0;
	token = ft_malloc(sizeof(t_token), head);
	if (quote == S_QUOTE)
		while (str[j] && str[j] != '\'')
			j++;
	else if (quote == D_QUOTE)
		while (str[j] && str[j] != '\"')
			j++;
	else
		j = count_no_quotes(str);
	new = ft_malloc(sizeof(char) * (j + 1), head);
	ft_strlcpy(new, str, j + 1);
	if ((quote == D_QUOTE || quote == NO_QUOTE) && ft_strchr(new, '$'))
		token->expandable = 1;
	else
		token->expandable = 0;
	token->content = new;
	token->type = WORD;
	token->quoted = quote;
	*i += j;
	return (token);
}

void	insert_other_ops(t_token *token, char *str, t_alloc **head)
{
	char	*new;

	if (str[0] == '|')
		token->type = PIPE;
	else if (str[0] == '<')
		token->type = INPUT_DIRECTION;
	else if (str[0] == '>')
		token->type = OUTPUT_DIRECTION;
	new = ft_malloc(sizeof(char) * 2, head);
	ft_strlcpy(new, str, 2);
	token->content = new;
}

t_token	*insert_token_arr_op(int *i, char *str, t_alloc **head)
{
	t_token	*token;

	token = ft_malloc(sizeof(t_token), head);
	if (str[0] == '<' && str[1] == '<')
	{
		token->type = HERE_DOC;
		token->content = ft_strdup("<<", head);
		*i += 1;
	}
	else if (str[0] == '>' && str[1] == '>')
	{
		token->type = OUT_APPEND;
		token->content = ft_strdup(">>", head);
		*i += 1;
	}
	else
		insert_other_ops(token, str, head);
	token->quoted = 0;
	token->expandable = 0;
	*i += 1;
	return (token);
}

void	loop_token_arr(char *str, t_token **token_arr, t_alloc **head)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (!str[++i])
				break ;
			if (str[i - 1] == '\'')
				token_arr[j++] = insert_token_arr_word(&i, str + i, S_QUOTE, head);
			else if (str[i - 1] == '\"')
				token_arr[j++] = insert_token_arr_word(&i, str + i, D_QUOTE, head);
			i++;
		}
		else if (is_word_char(str[i]))
			token_arr[j++] = insert_token_arr_word(&i, str + i, NO_QUOTE, head);
		else if (to_handle(str[i]))
			token_arr[j++] = insert_token_arr_op(&i, str + i, head);
		else
			i++;
	}
	token_arr[j] = NULL;
}

t_token	**tokenize(char *str, t_alloc **head)
{
	int		tokens_count;
	t_token	**token_arr;

	tokens_count = count_tokens(str);
	token_arr = ft_malloc(sizeof(t_token *) * (tokens_count + 1), head);
	loop_token_arr(str, token_arr, head);
	return (token_arr);
}
