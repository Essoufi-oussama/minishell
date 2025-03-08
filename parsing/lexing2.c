/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:11:08 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/07 17:43:51 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_operation(int i)
{
	return (i == INPUT_DIRECTION || i == OUTPUT_DIRECTION
		|| i == OUT_APPEND || i == HERE_DOC || i == PIPE);
}

void	open_heredocs(t_redir **head, t_data *data)
{
	t_redir	*current;

	current = *head;
	while (current)
	{
		here_doc(current, data);
		unlink(current->here_doc_filename);
		if (g_in_readline == 4)
			return ;
		current = current->next;
	}	
}

void	handle_heredocs(t_token **tokens, t_data *data)
{
	t_redir	*head;
	t_redir	*new;
	int		i;

	i = 0;
	head = NULL;
	while (tokens[i])
	{
		if (tokens[i] && tokens[i]->type == HERE_DOC)
		{
			if (tokens[i + 1] && tokens[i + 1]->type == WORD)
			{
				i++;
				new = ft_malloc(sizeof(t_redir), data);
				new->name = get_full_name(tokens, &i, data);
				new->next = NULL;
				lstadd(&head, new);
			}
			else
				i++;
		}
		else
			i++;
	}
	open_heredocs(&head, data);
}
