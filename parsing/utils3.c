/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 23:52:26 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/12 19:50:53 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_lstclear_env(t_env **lst)
{
	t_env	*tmp;

	if (lst == NULL)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free((*lst)->env_var);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

void	free_exit2(t_data *data, int i)
{
	ft_lstclear_garbage(&data->alloc);
	ft_lstclear_env(&data->env);
	if (data->default_path)
		free(data->default_path);
	free(data);
	exit(i);
}

void	free_exit(t_data *data)
{
	printf("exit\n");
	ft_lstclear_garbage(&data->alloc);
	ft_lstclear_env(&data->env);
	if (data->default_path)
		free(data->default_path);
	if (data->pwd)
		free(data->pwd);
	free(data);
	exit(exit_stat(0, 0));
}

void	destroy_heredocs(t_data *data)
{
	int		i;
	t_redir	*curr;

	i = 0;
	while (data->commands[i])
	{
		curr = data->commands[i]->files;
		while (curr)
		{
			if (curr->type == HERE_DOC)
			{
				if (access(curr->here_doc_filename, F_OK))
					unlink(curr->here_doc_filename);
			}
			curr = curr->next;
		}
		i++;
	}
}

void	add_data_line(char *str, t_data *data)
{
	t_alloc	*new;

	new = malloc(sizeof(t_alloc));
	if (new == NULL)
		free_exit(data);
	new->addr = str;
	new->next = NULL;
	ft_lstadd_front(&data->alloc, new);
}
