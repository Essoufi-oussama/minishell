/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 20:25:03 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/08 18:08:47 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_and_exit_error(int *p)
{
	perror("error");
	close(p[1]);
	close(p[0]);
	exit(EXIT_FAILURE);
}

void	close_and_exit_success(int *p, pid_t *pid, pid_t *pid2)
{
	close(p[1]);
	close(p[0]);
	waitpid(*pid, NULL, 0);
	waitpid(*pid2, NULL, 0);
	exit(EXIT_SUCCESS);
}

void	ft_error(char *str)
{
	if (str == NULL)
		exit(1);
	write(2, str, ft_strlen(str));
	exit(EXIT_FAILURE);
}
