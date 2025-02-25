/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infile_outfile.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 22:40:12 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/02/23 15:29:06 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void outfile_cas(t_command *command)
{
        int output;

        t_redir *outfiles = command->outfiles;
        if(outfiles && (outfiles->type == OUTPUT_DIRECTION || outfiles->type == OUT_APPEND))
        {
            while(outfiles)
            {
                if(outfiles->type == OUTPUT_DIRECTION)
                    output = open(outfiles->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if(outfiles->type == OUT_APPEND)
                output = open(outfiles->name, O_CREAT | O_WRONLY | O_APPEND, 0644);
                if(!output)
                    exit(1);
                if(outfiles->next)
                    close(output);
                outfiles = outfiles->next;
            }
            close(1);
            dup(output);
            close(output);
		}
    
}
void infile_cas(t_command *command)
{
    int input;
	t_redir *infiles = command->infiles;
	if (infiles == NULL)
		return;
	while(infiles)
	{
		if (infiles->type == INPUT_DIRECTION)
			input = open(infiles->name, O_RDONLY, 0644);
		else
			input = infiles->here_doc_fd;
		if(input == -1)
		{
			perror("eruer");	
			exit(1);
		}
		if(infiles->next)
			close(input);
		infiles = infiles->next;
	}
	dup2(input, 0);
	close(input);
}
