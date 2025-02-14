/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 12:09:41 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/14 20:49:17 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
typedef enum e_tokens_types
{
	PIPE ,
	INPUT_DIRECTION ,
	OUTPUT_DIRECTION ,
	OUT_APPEND ,
	HERE_DOC ,
	WORD ,
	SPECIAL_CHAR
}	t_tokens_types;

typedef struct s_token
{
	int		type;
	int		quoted;
	int		expandable;
	char	*content;
} t_token;


typedef enum e_quote
{
	NO_QUOTE,
	S_QUOTE,
	D_QUOTE,
} t_quote;

int	count_tokens(char *str);
t_token **tokenize(char *str);
void	check_tokens(t_token **tokens);
int	check_quotes(char *str);
int	is_operation(int i);
int	check_special_char(t_token **tokens);
void	insert_other_ops(t_token *token, char *str);
int is_word_char(char c);
int 	is_special(char c);
int to_handle(char c);
int handle_quotes(char *str, int *i, char quote_char);
int count_no_quotes(char *str);
int	check_consecutive_expressions(t_token **tokens);
int	check_end_and_pipe(t_token **tokens);
void	loop_token_arr(char *str, t_token **token_arr);

size_t	ft_strlcpy(char *dest, const char *src, size_t dstsize);
char	*ft_strrchr(const char *s, int c);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s1);
int	ft_isalpha(int c);
int	ft_isdigit(int c);
int	ft_isalnum(int c);
#endif