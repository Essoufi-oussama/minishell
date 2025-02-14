/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 12:09:41 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/14 16:37:48 by oessoufi         ###   ########.fr       */
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
	SINGLE_QUOTE ,
	WORD ,
	DOUBLE_QUOTE, 
	// DOLLAR,
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
void	check_tokens(int *tokens, int token_size);
int	check_quotes(char *str);
int	is_operation(int i);
int	check_special_char(int i , int *tokens);
void	insert_other_ops(t_token *token, char *str);
int is_word_char(char c);
int 	is_special(char c);
int to_handle(char c);
void	count_for_quotes(int *i, int *count, char *str);
int count_no_quotes(char *str);


size_t	ft_strlcpy(char *dest, const char *src, size_t dstsize);
char	*ft_strrchr(const char *s, int c);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s1);
#endif