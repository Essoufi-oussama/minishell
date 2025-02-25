/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:40:59 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/24 20:38:34 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_quotes(char *str, t_data *data)
{
	int		i;
	char	quote;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				i++;
			if (!str[i])
			{
				printf("Syntax error unclosed quote\n");
				data->exit_status = 2;
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	is_operation(int i)
{
	return (i == INPUT_DIRECTION || i == OUTPUT_DIRECTION
		|| i == OUT_APPEND || i == HERE_DOC || i == PIPE);
}

void	open_heredocs(t_redir **head, t_data *data)
{
	t_redir *current;
	pid_t 	pid;
	int fd;

	current = *head;
	while(current)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(1);
		}
		else if (pid == 0)
		{
			fd = here_doc(current, data);
			close(fd);
			exit(0);
		}
		else
			wait(NULL);
		current = current->next;
	}	
}

void	handle_heredocs(t_token **tokens, t_data *data)
{
	t_redir	*head;
	t_redir	*new;
	int	i;

	i = 0;
	head = NULL;
	while(tokens[i])
	{
		if (tokens[i] && tokens[i]->type == HERE_DOC)
		{
			if (tokens[i + 1] && tokens[i + 1]->type == WORD)
			{
				i++;
				new = ft_malloc(sizeof(t_redir), data);
				new->name = get_full_name(tokens, &i, data);
				new->next = NULL;
				ft_lstadd_back(&head, new);
			}
			else
				i++;
		}
		else
			i++;
	}
	open_heredocs(&head, data);
}

void	handle_heredocs2(t_token **tokens, int stop, t_data *data)
{
	t_redir	*head;
	t_redir	*new;
	int	i;

	i = 0;
	head = NULL;
	while(tokens[i] && i < stop)
	{
		if (tokens[i] && tokens[i]->type == HERE_DOC)
		{
			if (tokens[i + 1] && tokens[i + 1]->type == WORD)
			{
				i++;
				new = ft_malloc(sizeof(t_redir), data);
				new->name = get_full_name(tokens, &i, data);
				new->next = NULL;
				ft_lstadd_back(&head, new);
			}
			else
				i++;
		}
		else
			i++;
	}
	open_heredocs(&head, data);
}

int	check_consecutive_expressions(t_token **tokens, t_data *data)
{
	int		i;
	t_token	*previous;

	i = 0;
	if (tokens[0]->type == INPUT_DIRECTION)
		if (check_end_and_pipe(tokens, data) == 1)
			return (1);
	previous = tokens[i++];
	while (tokens[i])
	{
		if (is_operation(previous->type) && is_operation(tokens[i]->type))
		{
			if (previous->type != PIPE || (previous->type == PIPE && tokens[i]->type == PIPE))
			{
				printf("syntax error near unexpected token '%s'\n",
				tokens[i]->content);
				handle_heredocs2(tokens, i - 2, data);
				data->exit_status = 2;
				return (1);
			}
		}
		previous = tokens[i];
		i++;
	}
	return (0);
}

int	check_end_and_pipe(t_token **tokens, t_data *data)
{
	int	i;

	i = 0;
	while (tokens[i])
		i++;
	if (tokens[0]->type == PIPE)
	{
		printf("synatx error near unexpected token '|'\n");
		data->exit_status = 2;
		return (1);
	}
	if (is_operation(tokens[i - 1]->type) && tokens[i - 1]->type != PIPE)
	{
		handle_heredocs(tokens, data);
		printf("syntax error near unexpected token 'newline'\n");
		data->exit_status = 2;
		return (1);
	}
	return (0);
}

int	check_consecutive_expressions2(t_token **tokens, t_data *data)
{
	int		i;
	t_token	*previous;

	i = 0;
	if (tokens[0]->type == INPUT_DIRECTION)
		if (check_end_and_pipe(tokens, data) == 1)
			return (1);
	if (tokens[0]->type == PIPE)
	{
			printf("syntax error near unexpected token '%s'\n",
			tokens[0]->content);
			data->exit_status = 2;
			return (1);
	}
	previous = tokens[i++];
	while (tokens[i])
	{
		if (is_operation(previous->type) && is_operation(tokens[i]->type))
		{
			if (previous->type != PIPE || (previous->type == PIPE && tokens[i]->type == PIPE))
			{
				printf("syntax error near unexpected token '%s'\n",
				tokens[i]->content);
				handle_heredocs2(tokens, i - 2, data);
				data->exit_status = 2;
				return (1);
			}
		}
		previous = tokens[i];
		i++;
	}
	return (0);
}

int	lexing_new_stuff(t_token **tokens, t_data *data)
{
	int	i;

	i = 0;
	if (tokens[i] == NULL)
		return (0);
	if (check_consecutive_expressions2(tokens, data) == 1)
		return (0);
	if (check_end_and_pipe(tokens, data) == 1)
		return (0);
	return (1);
}

int	lexing(t_token **tokens, t_data *data)
{
	int	i;

	i = 0;
	if (tokens[i] == NULL)
		return (0);
	if (check_consecutive_expressions(tokens, data) == 1)
		return (0);
	if (check_end_and_pipe(tokens, data) == 1)
		return (0);
	return (1);
}
