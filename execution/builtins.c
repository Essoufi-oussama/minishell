/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 20:13:56 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/02/25 14:01:14 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void echo(char **str)
{
    int i = 1;
    int option_flag = 0;
    if (str[1] == NULL)
    {
        printf("\n");
        return;
    }
    if (strcmp(str[1], "-n") == 0)
    {
        option_flag = 1;
        i = 2;
    }
    while (str[i] && strcmp(str[i], "-n") == 0)
        i++;
    while (str[i])
    {
        printf("%s", str[i]);
        if (str[i + 1])
            printf(" ");
        i++;
    }
    if (option_flag == 0)
        printf("\n");
}

void cd(char **str)
{
    if(str[1] != NULL && str[2] != NULL)
    {
        printf("cd: too many arguments\n");
        return;
    }
    if (str[1] == NULL)
    {
        str[1] = getenv("HOME");
        if (str[1] == NULL)
        {
            printf("cd: No home directory found\n");
            return;
        }
    }
    if (chdir(str[1]))
    {
        perror("cd");
        return;
    }
}

void pwd()
{
    char s[100];
    char *path = getcwd(s, 100);
    if (!path)
        printf("error\n");
    else
        printf("%s\n", path);
}


void env(char **str, t_data *data)
{
	t_env *head;

	head = data->env;
    if (str[1] != NULL)
    {
        printf("No such file or directory\n");
        return;
    }
    while (head)
    {
        if(ft_strchr(head->env_var,'='))
            printf("%s\n", head->env_var);
        head = head->next;
    }
}

void exit_program(int status)
{
    printf("exit\n");
    exit(status);
}
