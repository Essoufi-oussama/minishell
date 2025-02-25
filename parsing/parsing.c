/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:59:54 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/24 21:11:58 by oessoufi         ###   ########.fr       */
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
			if (tokens[i]->split_later == 1)
				count += word_count(tokens[i]->content, ' ');
			else
				count++;
			i++;
			while (i < tokens_count && tokens[i]->part_of_previous)
			{
				if (tokens[i]->split_later == 1)
					count += word_count(tokens[i]->content, ' ');
				i++;
			}
		}
	}
	return (count);
}

int here_doc(t_redir *infile, t_data *data)
{
    int fd[2];
    char *line;
	char	*tmp;
	char *limiter = infile->name;
    if(pipe(fd) == -1)
    {
        perror("pipe");
        return (-1);
    }
    while(1)
    {
        line = readline("> ");
        if(line == NULL || ft_strcmp(line, limiter) == 0)
        {
            free(line);
            break;
        }
		tmp = line;
        if (infile->here_doc_expandable)
            line = handle_quoted_token(line, data);
        if(write(fd[1], line, ft_strlen(line)) == -1 || write(fd[1], "\n", 1) == -1)
        {
            perror("write");
            free(line);
            close(fd[0]);
            close(fd[1]);
            return (-1);
        }
        free(tmp);
    }
    
    close(fd[1]);
    return (fd[0]);
}

t_redir	*insert_in_heredoc(t_token **tokens, int *i, t_data *data)
{
	t_redir *infile;

	infile = ft_malloc(sizeof(t_redir), data);
	infile->type = tokens[*i]->type;
	(*i)++;
	infile->name = get_full_name_in(tokens, infile, i, data);
	infile->here_doc_fd = here_doc(infile, data);
	infile->next = NULL;
	return(infile);
}

void	split_this_sh(char *str, char **command, int *j, t_data *data)
{
	char	**split_str;
	int		i;

	i = 0;
	split_str = ft_split(str, ' ', data);
	while(split_str[i])
	{
		command[*j] = split_str[i];
		i++;
		(*j)++;
	}
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
		if (tokens[i]->type == INPUT_DIRECTION )
			ft_lstadd_back(&command->infiles , insert_in(tokens, &i, data));
		else if (tokens[i]->type == HERE_DOC)
			ft_lstadd_back(&command->infiles , insert_in_heredoc(tokens, &i, data));
		else if (tokens[i]->type == OUT_APPEND || tokens[i]->type == OUTPUT_DIRECTION)
			ft_lstadd_back(&command->outfiles , insert_out(tokens, &i, data));
		else if (tokens[i]->type == WORD)
		{
			if (tokens[i]->split_later == 1)
				split_this_sh(get_full_name(tokens, &i, data), command->args, &j, data);
			else
				command->args[j++] = get_full_name(tokens, &i, data);
		}
		else
			i++;
	}
	command->args[j] = NULL;
	command->args_count = j;
	return (command);
}

void	parse(t_data *data)
{
	int i;
	int	j;
	t_token	**tokens;

	data->command_count = count_commands(data->tokens);
	i = 0;
	tokens = data->tokens;
	data->commands = ft_malloc(sizeof(t_command *) * (data->command_count + 1), data);
	while (i < data->command_count)
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
}

void	join_both_arrays(t_data *data)
{
	t_command **command;
	int	i ;
	int	j ;
	
	i =0;
	command = ft_malloc(sizeof(t_command *) * (data->command_count + data->readline_command_count), data);
	while(data->commands[i])
	{
		command[i] = data->commands[i];
		i++;
	}
	i--;
	j = 0;
	while(data->readline_commands[j])
		command[i++] = data->readline_commands[j++];
	command[i] = NULL;
	data->command_count = i;
	data->commands = command;
}

void	parsing_new_stuff(t_data *data)
{
	int i;
	int	j;
	t_token	**tokens;

	data->readline_command_count = count_commands(data->readline_tokens);
	i = 0;
	tokens = data->readline_tokens;
	data->readline_commands = ft_malloc(sizeof(t_command *) * (data->readline_command_count + 1), data);
	while (i < data->readline_command_count)
	{
		j = 0;
		while(tokens[j] && tokens[j]->type != PIPE)
			j++;
		data->readline_commands[i++] = build_command(data, tokens, j);
		if (tokens[j] && tokens[j]->type == PIPE)
			j++;
		tokens = tokens + j;
	}
	data->readline_commands[i] = NULL;
	join_both_arrays(data);
}

