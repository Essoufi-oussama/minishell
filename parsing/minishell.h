/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 12:09:41 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/13 14:57:12 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
typedef enum e_tokens
{
	PIPE ,
	WHITE_SPACE ,
	INPUT_DIRECTION ,
	OUTPUT_DIRECTION ,
	OUT_APPEND ,
	HERE_DOC ,
	SINGLE_QUOTE ,
	WORD ,
	DOUBLE_QUOTE, 
	DOLLAR,
	SPECIAL_CHAR
}	t_tokens;


int	count_tokens(char *str);
int *tokenize(char *str, int *token_size);
void	check_tokens(int *tokens, int token_size);
int	check_quotes(int i, int *tokens);
int	is_operation(int i);
int	check_special_char(int i , int *tokens);
#endif