/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:28:07 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/23 19:57:25 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

char	*ft_strdup(const char *s1, t_data *data)
{
	size_t	i;
	char	*s2;

	i = ft_strlen(s1);
	s2 = ft_malloc(i + 1, data);
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

int	ft_isprint(int c)
{
	if (c > 32 && c <= 126)
		return (1);
	return (0);
}

char	*ft_strjoin(char const *s1, char const *s2, t_data *data)
{
	char	*str;
	size_t	i;
	size_t	j;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	if (s1 == NULL)
		return (ft_strdup(s2, data));
	else if (s2 == NULL)
		return (ft_strdup(s1, data));
	str = ft_malloc(ft_strlen(s1) + ft_strlen(s2) + 1, data);
	if (str == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		str[i++] = s2[j++];
	str[i] = '\0';
	return (str);
}

char *ft_strtrim(char const *s1, t_data *data)
{
    size_t start;
    size_t end;
    char *str;

    if (!s1)
        return (NULL);
    start = 0;
    end = ft_strlen(s1);
    if (end == 0)
        return (ft_strdup("", data));
    end--;
    while (s1[start] && s1[start] == ' ')
        start++;
    while (end > start && s1[end] == ' ')
        end--;
    str = ft_malloc(end - start + 2, data);
    if (!str)
        return (NULL);
    ft_strlcpy(str, &s1[start], end - start + 2);
    return (str);
}
