/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:40:59 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/13 14:37:19 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quotes(int i, int *tokens)
{
	while(tokens[i] != -1)
	{
		if (tokens[i] == SINGLE_QUOTE)
		{
			i++;
			while(tokens[i] != -1 && tokens[i] != SINGLE_QUOTE)
				i++;
			if (tokens[i] == SINGLE_QUOTE)
				i++;
			else if (tokens[i] == -1)
			{
				printf("Syntax error unclosed quote \n");
				return (1);
			}
		}
		else if (tokens[i] == DOUBLE_QUOTE)
		{
			i++;
			while(tokens[i] != -1 && tokens[i] != DOUBLE_QUOTE)
				i++;
			if (tokens[i] == DOUBLE_QUOTE)
				i++;
			else if (tokens[i] == -1)
			{
				printf("Syntax error unclosed quote\n");
				return (1);
			}
		}
		else
			i++;
	}
	return (0);
}

int	check_special_char(int i , int *tokens)
{
	int inside_quotes = 0;
	while(tokens[i] != - 1)
	{
		if (tokens[i] == SPECIAL_CHAR && inside_quotes == 0)
		{
			printf("syntax error non supported character\n");
			return (1);
		}
		if ((tokens[i] == DOUBLE_QUOTE || tokens[i] == SINGLE_QUOTE ) && inside_quotes == 0)
			inside_quotes = 1;
		else if ((tokens[i] == DOUBLE_QUOTE || tokens[i] == SINGLE_QUOTE )&& inside_quotes == 1)
			inside_quotes = 0;
		i++;
	}
	return (0);
}


int	is_operation(int i)
{
	return (i == INPUT_DIRECTION || i == OUTPUT_DIRECTION || i == OUT_APPEND || i == HERE_DOC || i == PIPE);
}

static void	print_token(int i)
{
	if (i == INPUT_DIRECTION)
		printf("'<'\n");
	else if (i == OUTPUT_DIRECTION)
		printf("'>'\n");
	else if (i == OUT_APPEND)
		printf("'>>'\n");
	else if (i == HERE_DOC)
		printf("'<<'\n");
	else if (i == PIPE)
		printf("'|'\n");
}

int	check_consecutive_expressions(int i, int *tokens)
{
	int previous = tokens[i];

	i = 1;
	while(tokens[i] != -1)
	{
		while(tokens[i] != -1 && tokens[i] == WHITE_SPACE)
			i++;
		if (tokens[i] == -1)
			break ;
		if (is_operation(previous) && is_operation(tokens[i]))
		{
			printf("syntax error near unexpected token ");
			print_token(tokens[i]);
			return (1);
		}
		previous = tokens[i];
		i++;
	}
	return(0);
}

int	check_end_and_pipe(int *tokens, int size)
{
	int	i;

	while(tokens[size] == WHITE_SPACE && size >= 0)
		size--;
	if (tokens[size] == HERE_DOC || tokens[size] == OUT_APPEND || tokens[size] == OUTPUT_DIRECTION ||tokens[size] == INPUT_DIRECTION)
	{
		printf("syntax error near unexpected token 'newline'\n");
		return (1);
	}
	i = 0;
	while(i < size && tokens[i] == WHITE_SPACE)
		i++;
	if (tokens[i] == PIPE)
	{
		printf("synatx error near unexpected token '|'\n");
		return (1);
	}
	return (0);
}

void	check_tokens(int *tokens, int size)
{
	int	i;

	i = 0;
	while(tokens[i] == WHITE_SPACE)
		i++;
	if (i == size)
		return ;
	if (check_quotes(i , tokens) == 1)
		return ;
	if (check_special_char(i , tokens) == 1)
		return ;
	if (check_consecutive_expressions(i, tokens) == 1)
		return ;
	if (check_end_and_pipe(tokens, size - 1)  == 1)
		return ;
}