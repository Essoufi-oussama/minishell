/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:59:54 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/18 22:34:47 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_commands(t_token **tokens)
{
	int i;
	int count;

	count = 1;
	i = 0;
	while(tokens[i])
	{
		if (tokens[i]->type == PIPE)
			count++;
		i++;
	}
	return (count);
}

void	command_init(t_command *command)
{
	command->args = NULL;
	command->infiles = NULL;
	command->outfiles = NULL;
}

t_redir	*ft_lstlast(t_redir *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_redir **lst, t_redir *new)
{
	t_redir	*last;

	if (new == NULL || lst == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = ft_lstlast(*lst);
	last->next = new;
}
char	*get_full_name_in(t_token **tokens, t_redir *infile, int *i, t_data *data)
{
	char	*name;

	infile->here_doc_expandable = 1;
	name = tokens[*i]->content;
	if (tokens[*i]->quoted)
		infile->here_doc_expandable = 0;
	(*i)++;
	while(tokens[*i] && tokens[*i]->part_of_previous)
	{
		name = ft_strjoin(name, tokens[*i]->content, data);
		if (tokens[*i]->quoted)
			infile->here_doc_expandable = 0;
		(*i)++;
	}
	return (name);
}

char	*get_full_name(t_token **tokens, int *i, t_data *data)
{
	char	*name;

	name = tokens[*i]->content;
	(*i)++;
	while(tokens[*i] && tokens[*i]->part_of_previous)
	{
		name = ft_strjoin(name, tokens[*i]->content, data);
		(*i)++;
	}
	printf("%s\n", name);
	return (name);
}

t_redir	*insert_in(t_token **tokens, int *i, t_data *data)
{
	t_redir *infile;

	infile = ft_malloc(sizeof(t_redir), data);
	infile->type = tokens[*i]->type;
	(*i)++;
	infile->name = get_full_name_in(tokens, infile,i, data);
	infile->next = NULL;
	return(infile);
}

t_redir	*insert_out(t_token **tokens, int *i, t_data *data)
{
	t_redir *outfile;

	outfile = ft_malloc(sizeof(t_redir), data);
	outfile->type = tokens[*i]->type;
	(*i)++;
	outfile->name = get_full_name(tokens, i, data);
	outfile->here_doc_expandable = 0;
	outfile->next = NULL;
	return(outfile);
}

int	args_count(t_token **tokens, int tokens_count)
{
	int i;
	int	count;

	i = 0;
	count = 0;
	while(i < tokens_count)
	{
		if (is_operation(tokens[i]->type))
		{
			i += 2;
			while (i < tokens_count && tokens[i]->part_of_previous)
				i++;
		}
		else
		{
			count++;
			i++;
			while (i < tokens_count && tokens[i]->part_of_previous)
				i++;
		}
	}
	return (count);
}

t_command	*build_command(t_data *data, t_token **tokens, int count)
{
	int	i;
	int j;
	t_command *command;

	command = ft_malloc(sizeof(t_command), data);
	command_init(command);
	i = 0;
	j = 0;
	command->args = ft_malloc(sizeof(char *) * (args_count(tokens, count) + 1), data);
	while(i < count)
	{
		if (tokens[i]->type == INPUT_DIRECTION || tokens[i]->type == HERE_DOC)
			ft_lstadd_back(&command->infiles , insert_in(tokens, &i, data));
		else if (tokens[i]->type == OUT_APPEND || tokens[i]->type == OUTPUT_DIRECTION)
			ft_lstadd_back(&command->outfiles , insert_out(tokens, &i, data));
		else if (tokens[i]->type == WORD)
			command->args[j++] = get_full_name(tokens, &i, data);
		else
			i++;
	}
	command->args[j] = NULL;
	return (command);
}

void	parse(t_data *data)
{
	int i;
	int	j;
	int count;
	t_token	**tokens;

	count = count_commands(data->tokens);
	i = 0;
	tokens = data->tokens;
	data->commands = ft_malloc(sizeof(t_command *) * (count_commands(tokens) + 1), data);
	while (i < count)
	{
		j = 0;
		while(tokens[j] && tokens[j]->type != PIPE)
			j++;
		data->commands[i++] = build_command(data, tokens, j);
		if (tokens[j] && tokens[j]->type == PIPE)
			j++;
		tokens = tokens + j;
	}
	data->commands[i] = NULL;
	i = 0;
	// while(data->commands[i])
	// {
	// 	j = 0;
	// 	while(data->commands[i]->args[j])
	// 		printf("%s\n", data->commands[i]->args[j++]);
	// 	i++;
	// }
}
