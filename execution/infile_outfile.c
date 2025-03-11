/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infile_outfile.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 22:40:12 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/11 16:48:48 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_error_status(char *str, const char *msg)
{
	write(2, "minihell: ", 11);
	write(2, str, ft_strlen(str));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

static	void	check_dir(char *str, t_data *data, t_alloc **head)
{
	struct stat	path_stat;

	if (stat(str, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		print_error_status(str, "Is a directory");
		free_exit_child(data, head, 1);
	}
}

void	outfile_cas(t_redir *outfiles, t_data *data, t_alloc **head,int flag)
{
	int	output;

	if (outfiles->ambigious)
	{
		print_error_status(outfiles->name, "ambiguous redirect");
		free_exit_child(data, head, 1);
	}
	check_dir(outfiles->name, data, head);
	if (outfiles->type == OUTPUT_DIRECTION)
		output = open(outfiles->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (outfiles->type == OUT_APPEND)
		output = open(outfiles->name, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (!output)
	{
		perror(outfiles->name);
		free_exit_child(data, head, 1);
	}
	if(flag == 0)
	{
		close(1);
		if (dup(output) == -1)
		{
			perror("minihell");
			close(output);
			free_exit_child(data, head, 1);
		}
	}
	close(output);
}

void	infile_cas(t_redir *infiles, t_data *data, t_alloc **head,int flag)
{
	int	input;

		if (infiles->ambigious && infiles->type == INPUT_DIRECTION)
		{
			print_error_status(infiles->name, "ambiguous redirect");
			free_exit_child(data, head, 1);
		}
	if (infiles->type == INPUT_DIRECTION)
		input = open(infiles->name, O_RDONLY, 0644);
	else
		input = open(infiles->here_doc_filename, O_RDWR, 0644);
	if (input == -1)
	{
		write(2, "minihell: ", 11);
		perror(infiles->name);
		free_exit_child(data, head, 1);
	}
	if(flag == 0)
	{
		close(0);
		if (dup(input) == -1)
		{
			perror("minihell: ");
			close(input);
			free_exit_child(data, head, 1);
		}
	}
	close(input);
}

void	files(t_command *command, t_data *data, t_alloc **head, int flag)
{
	t_redir	*files;

	files = command->files;
	while (files)
	{
		if (files->type == INPUT_DIRECTION || files->type == HERE_DOC)
			infile_cas(files, data, head,flag);
		else if (files->type == OUTPUT_DIRECTION || files->type == OUT_APPEND)
			outfile_cas(files, data, head,flag);
		files = files->next;
	}
}
