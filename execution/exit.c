/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:30:41 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/11 21:49:06 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

long	ft_atol(char *str, t_data *data)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ')
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result > (LONG_MAX - (str[i] - '0')) / 10)
			(printf("exit\nexit: %s: numeric argument required\n", str),
				free_exit2(data, 2));
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

long	ft_atol2(char *str, t_data *data, t_alloc **head)
{
	long	result;
	int		sign;
	int		i;

	(1) && (result = 0, sign = 1, i = 0);
	while (str[i] == ' ')
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result > (LONG_MAX - (str[i] - '0')) / 10)
		{
			print_error_status(str, ":numeric argument required\n");
			free_exit_child(data, head, 2);
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

int	valid(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i] != '\0')
	{
		if (!isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	exit_child(char **str, t_data *data, t_alloc **head)
{
	long	num;

	if (str[1] != NULL)
	{
		if (str[2] != NULL)
		{
			if (!valid(str[1]))
			{
				write(2, "exit: ", 7);
				print_error_status(str[1], ": numeric argument required\n");
				free_exit_child(data, head, 2);
			}
			write(2, "exit: too many arguments\n", 26);
			free_exit_child(data, head, 1);
		}
		else if (!valid(str[1]))
		{
			write(2, "exit: ", 7);
			print_error_status(str[1], ": numeric argument required\n");
			free_exit_child(data, head, 2);
		}
		else
		{
			num = ft_atol2(str[1], data, head);
			free_exit_child(data, head, num % 256);
		}
	}
	else
		free_exit_child(data, head, 0);
}

void	exit_program(char **str, t_data *data)
{
	long	num;

	if (str[1] != NULL)
	{
		if (str[2] != NULL)
		{
			if (!valid(str[1]))
			{
				printf("exit\nexit: %s: numeric argument required\n", str[1]);
				free_exit2(data, 2);
			}
			printf("exit\n");
			printf("exit: too many arguments\n");
			exit_stat(1, 1);
			return ;
		}
		else if (!valid(str[1]))
		{
			printf("exit\nexit: %s: numeric argument required\n", str[1]);
			free_exit2(data, 2);
		}
		else
		{
			num = ft_atol(str[1], data);
			printf("exit\n");
			free_exit2(data, num % 256);
		}
	}
	else
		(printf("exit\n"), free_exit2(data, 0));
}
