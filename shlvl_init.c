#include "minishell.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	c;

	if (n == INT_MIN)
	{
		write(fd, "-2147483648", 12);
		return ;
	}
	if (n < 0)
	{
		write(fd, "-", 1);
		n *= -1;
	}
	if (n > 9)
	{
		ft_putnbr_fd(n / 10, fd);
	}
	c = (n % 10) + '0';
	write(fd, &c, 1);
}

int	ft_atoi(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if ((*str == '-' || *str == '+') && ft_isdigit(*(str + 1)))
	{
		sign = -1 * (*str == '-') + 1 * (*str == '+');
		str++;
	}
	while (*str >= 48 && *str <= 57)
	{
		result = result * 10 + sign * (*str - 48);
		if (result > INT_MAX || result < INT_MIN)
			return (0);
		str++;
	}
	return (result);
}

int	check_for_non_digits(char *str)
{
	int	sign;

	sign = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '+' || *str == '-')
	{
		str++;
		sign = 1;
	}
	if (!(*str >= '0' && *str <= '9') && sign)
		return (1);
	while (*str >= '0' && *str <= '9')
		str++;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str != '\0')
		return (1);
	return (0);
}

void	shlvl_warning(int number)
{

	write(2, "minihell: warning: minishell level (", 37);
	ft_putnbr_fd(number, 2);
	write(2, ") too high, resetting to 1\n", 28);
}

static void	shlvl_init_when_shlvl_exists(char *str, t_data *data)
{
	char	*new_number;
	char	*shlvl;
	int		number;
	t_env	*current;

	shlvl = ft_strchr(str, '=');
	if (shlvl == NULL || check_for_non_digits(shlvl))
		number = 0;
	else
		number = ft_atoi(shlvl) + 1;
	if (number < 0)
		number = -1;
	else if (number > 998)
	{
		shlvl_warning(number + 1);
		number = 0;
	}
	new_number = ft_itoa(number + 1, data);
	current = data->env;
	while(current)
	{
		if (ft_strcmp_env(current->env_var, "SHLVL") == 0)
			break;
		current = current->next;
	}
	free(current->env_var);
	current->env_var = ftt_strdup(ft_strjoin("SHLVL=", new_number, data));
	if (current->env_var == NULL)
		free_exit(data);
}

void	shlvl_init(t_data *data)
{
	char *str;

	str = ft_getenv2("SHLVL", data);
	if (!str)
	{
		printf("HELLO\n");
		check_add("SHLVL=1", data);
	}
	else
		shlvl_init_when_shlvl_exists(str, data);
}
