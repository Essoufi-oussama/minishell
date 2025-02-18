/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:33:05 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/18 14:33:56 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	set_previous_part(t_token *token, int *i, char *str, int quote)
{
	token->part_of_previous = 0;
	if ((quote == D_QUOTE || quote == S_QUOTE) && *i - 1 > 0
		&& is_word_char(str[*i - 2]))
		token->part_of_previous = 1;
	else if (quote == NO_QUOTE && *i > 0 && is_word_char(str[*i - 1]))
		token->part_of_previous = 1;
}

static int	get_word_length(char *str, int *i, int quote)
{
	int	j;

	j = 0;
	if (quote == S_QUOTE)
		while (str[*i + j] && str[*i + j] != '\'')
			j++;
	else if (quote == D_QUOTE)
		while (str[*i + j] && str[*i + j] != '\"')
			j++;
	else
		j = count_no_quotes(str + *i);
	return (j);
}

t_token	*insert_token_arr_word(int *i, char *str, int quote, t_data *data)
{
	char		*new;
	int			j;
	t_token		*token;

	token = ft_malloc(sizeof(t_token), data);
	set_previous_part(token, i, str, quote);
	j = get_word_length(str, i, quote);
	new = ft_malloc(sizeof(char) * (j + 1), data);
	ft_strlcpy(new, str + *i, j + 1);
	token->expandable = ((quote == D_QUOTE || quote == NO_QUOTE)
			&& ft_strchr(new, '$'));
	token->content = new;
	token->type = WORD;
	token->quoted = quote;
	*i += j;
	return (token);
}

void	insert_other_ops(t_token *token, char *str, t_data *data)
{
	char	*new;

	if (str[0] == '|')
		token->type = PIPE;
	else if (str[0] == '<')
		token->type = INPUT_DIRECTION;
	else if (str[0] == '>')
		token->type = OUTPUT_DIRECTION;
	new = ft_malloc(sizeof(char) * 2, data);
	ft_strlcpy(new, str, 2);
	token->content = new;
}

t_token	*insert_token_arr_op(int *i, char *str, t_data *data)
{
	t_token	*token;

	token = ft_malloc(sizeof(t_token), data);
	if (str[0] == '<' && str[1] == '<')
	{
		token->type = HERE_DOC;
		token->content = ft_strdup("<<", data);
		*i += 1;
	}
	else if (str[0] == '>' && str[1] == '>')
	{
		token->type = OUT_APPEND;
		token->content = ft_strdup(">>", data);
		*i += 1;
	}
	else
		insert_other_ops(token, str, data);
	token->quoted = 0;
	token->expandable = 0;
	token->part_of_previous = 0;
	*i += 1;
	return (token);
}

void	loop_token_arr(char *str, t_token **token_arr, t_data *data)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (is_quote(str[i]))
		{
			if (!str[++i])
				break ;
			if (str[i - 1] == '\'')
				token_arr[j++] = insert_token_arr_word(&i, str, S_QUOTE, data);
			else if (str[i - 1] == '\"')
				token_arr[j++] = insert_token_arr_word(&i, str, D_QUOTE, data);
			i++;
		}
		else if (is_word_char(str[i]) && !is_quote(str[i]))
			token_arr[j++] = insert_token_arr_word(&i, str, NO_QUOTE, data);
		else if (to_handle(str[i]))
			token_arr[j++] = insert_token_arr_op(&i, str + i, data);
		else
			i++;
	}
	token_arr[j] = NULL;
}

void	tokenize(t_data *data)
{
	int		tokens_count;

	tokens_count = count_tokens(data->line);
	data->tokens = ft_malloc(sizeof(t_token *) * (tokens_count + 1), data);
	loop_token_arr(data->line, data->tokens, data);
}
