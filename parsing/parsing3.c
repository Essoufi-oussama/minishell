/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:38:33 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/08 22:38:31 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_full_name(t_token **tokens, int *i, t_data *data)
{
	char	*name;

	name = tokens[*i]->content;
	(*i)++;
	while (tokens[*i] && tokens[*i]->part_of_previous)
	{
		name = ft_strjoin(name, tokens[*i]->content, data);
		(*i)++;
	}
	return (name);
}

static int	count_token_words(t_token *token)
{
	if (token->split_later == 1)
		return (word_count(token->content, ' '));
	return (1);
}

int	arg_n(t_token **tokens, int tokens_count)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < tokens_count)
	{
		if (is_operation(tokens[i]->type))
		{
			i += 2;
			while (i < tokens_count && tokens[i]->part_of_previous)
				i++;
		}
		else
		{
			count += count_token_words(tokens[i]);
			i++;
			while (i < tokens_count && tokens[i]->part_of_previous)
			{
				count += count_token_words(tokens[i]);
				i++;
			}
		}
	}
	return (count);
}

char	*get_heredoc_name(t_data *data)
{
	static int	i = 0;
	char		*number;
	char		*name;

	i = (i + 2748) * 5256 % 2099980;
	number = ft_itoa(i, data);
	name = ft_strjoin("/tmp/.here_doc_limit", number, data);
	if (access(name, F_OK | X_OK) != -1)
	{
		while (access(name, F_OK | X_OK) != -1)
			name = ft_strjoin(name, "t", data);
	}
	i++;
	return (name);
}

void	write_heredoc(int fd, t_redir *infile, t_data *data)
{
	char	*line;
	char	*limiter;

	g_in_readline = 3;
	limiter = infile->name;
	while (1)
	{
		line = readline("> ");
		add_data_line(line, data);
		if (infile->here_doc_trim)
			line = ft_strtrim_tab(line, data);
		if (line == NULL || ft_strcmp(line, limiter) == 0)
			break ;
		if (infile->here_doc_expandable)
			line = expand_token(line, data);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
	close(fd);
	data->fd_write = -1;
}

void	here_doc(t_redir *infile, int *j, t_data *data)
{
	int		fd;
	int		fd_read;

	infile->here_doc_filename = get_heredoc_name(data);
	fd = open(infile->here_doc_filename, O_RDWR | O_CREAT | O_TRUNC, 0640);
	fd_read = open(infile->here_doc_filename, O_RDONLY);
	data->fds[*j] = fd_read;
	(*j)++;
	data->fd_write = fd;
	infile->here_doc_fd = fd_read;
	unlink(infile->here_doc_filename);
	write_heredoc(fd, infile, data);
}
