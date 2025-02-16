/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 12:09:41 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/16 13:43:10 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <string.h>

typedef enum e_tokens_types
{
	PIPE ,
	INPUT_DIRECTION ,
	OUTPUT_DIRECTION ,
	OUT_APPEND ,
	HERE_DOC ,
	WORD ,
}	t_tokens_types;

typedef struct s_token
{
	int		type;
	int		quoted;
	int		expandable;
	char	*content;
}	t_token;

typedef enum e_quote
{
	NO_QUOTE,
	S_QUOTE,
	D_QUOTE,
}	t_quote;

int		count_tokens(char *str);
t_token	**tokenize(char *str);
void	check_tokens(t_token **tokens);
int		check_quotes(char *str);
int		is_operation(int i);
int		check_special_char(t_token **tokens);
void	insert_other_ops(t_token *token, char *str);
int		is_word_char(char c);
int		is_special(char c);
int		to_handle(char c);
int		handle_quotes(char *str, int *i, char quote_char);
int		count_no_quotes(char *str);
int		check_consecutive_expressions(t_token **tokens);
int		check_end_and_pipe(t_token **tokens);
void	loop_token_arr(char *str, t_token **token_arr);

void	expanding(t_token **tokens);
char	*build_expanded_string(char *token, char *previous, int i);
char	*handle_quoted_token(char *token);
char	*expand_token(char *token);
char	*create_spaces(int count);
char	*join_tokens(char **strs, char *token, int j);
char	*get_expanded_value(char *token, int i, int *j);

size_t	ft_strlcpy(char *dest, const char *src, size_t dstsize);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s1);
int		ft_isprint(int c);
char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *s, char sep);

#endif