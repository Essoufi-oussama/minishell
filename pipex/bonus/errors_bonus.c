/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 20:28:22 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/04 17:15:19 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	close_and_exit_error(int *p)
{
	perror("error");
	close(p[1]);
	close(p[0]);
	exit(EXIT_FAILURE);
}

void	close_and_exit_success(int p)
{
	close(p);
	while (wait(NULL) > 0)
		;
	exit(EXIT_SUCCESS);
}

void	ft_error_sys(char *str)
{
	if (str == NULL)
		exit(1);
	perror(str);
	exit (EXIT_FAILURE);
}

void	ft_error(char *str)
{
	if (str == NULL)
		exit(1);
	write(2, str, ft_strlen(str));
	exit (EXIT_FAILURE);
}

void	ft_error2(char *str, int p)
{
	close(p);
	if (str == NULL)
		exit(1);
	perror(str);
	exit (EXIT_FAILURE);
}
