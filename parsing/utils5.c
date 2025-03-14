


#include "../minishell.h"

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
		str++;
	}
	return (result);
}
