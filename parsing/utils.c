/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:28:07 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/14 20:05:08 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (*s)
	{
		i++;
		s++;
	}
	return (i);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	src_len;

	i = 0;
	src_len = ft_strlen(src);
	if (dstsize != 0)
	{
		while (i < (dstsize - 1) && src[i] != '\0')
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = 0;
	}
	return (src_len);
}

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen(s);
	while (i >= 0)
	{
		if ((char) c == s[i])
			return ((char *) s + i);
		i--;
	}
	return (NULL);
}

char	*ft_strdup(const char *s1)
{
	size_t	i;
	char	*s2;

	i = ft_strlen(s1);
	s2 = (char *)malloc(i + 1);
	if (s2 == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = 0;
	return (s2);
}

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (1);
	return (0);
}

int is_word_char(char c)
{
    return (ft_isalnum(c) || c == '_' || c == '.' || c == '/' || 
            c == '-' || c == '$' || c == '+' || c == '@' || 
            c == ':' || c == ','|| c == '=');
}

int is_special(char c)
{
    return (c == '&' || c == ';' || c == '(' || c == '`' || 
            c == '{' || c == '}' || c == ')' || c == '*' || 
            c == '~' || c == '\\' || c == '%' || c == '!' || 
            c == '#' || c == '?'  || c == '[' || 
            c == ']' || c == '^' );
}

int to_handle(char c)
{
	return(c == '|' || c == '<' || c == '>' || is_special(c));
}
