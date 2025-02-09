/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 12:09:41 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/09 17:06:11 by oessoufi         ###   ########.fr       */
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
	INPUT_DIRECTION ,
	OUTPUT_DIRECTION ,
	OUT_APPEND ,
	HERE_DOC ,
	SINGLE_QUOTE ,
	WORD ,
	DOUBLE_QUOTE, 
	DOLLAR
}	t_tokens;

int	count_tokens(char *str);
int	*tokenize(char *line);

#endif