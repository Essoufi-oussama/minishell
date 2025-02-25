/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:26:24 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/02/23 10:01:43 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"


int ft_strncmp(const char *s1, const char *s2, size_t n)
{
    size_t i;

    i = 0;
    while (i < n && (s1[i] || s2[i]))
    {
        if (s1[i] != s2[i])
            return ((unsigned char)s1[i] - (unsigned char)s2[i]);
        i++;
    }
    return (0);
}

void free_ultimate(char **args)
{
    int i;

    if (!args)
        return;
    i = 0;
    while (args[i])
        free(args[i++]);
    free(args);
}
int	ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
    if(s1 == NULL)
    {
        return(-1);
    }
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}
