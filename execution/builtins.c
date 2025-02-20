/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 19:49:21 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/02/20 15:34:40 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// -nnnnn
void echo(char **str)

{
    int i = 1;
    int option_flag = 0;
    if(str[1] == NULL)
    {
        printf("\n");
        return;
    }
    if(strcmp(str[1],"-n") == 0)
    {
        option_flag = 1;
        i = 2;
    }
    while(str[i] && str[i] == "-n")
        i++;
    while(str[i])
    {
        printf("%s",str[i]);
        if(str[i + 1])
           printf(" ");
        i++;
    }
    if(option_flag == 0)
        printf("\n");
}
void cd(char **str)
{
    if(str[1] == NULL)
    {
        str[1] = getenv("HOME");
        if(str[1] == NULL)
            printf("cd: No home directory found\n");
    }
    if(chdir(str[1]) != 0)
        perror("cd");
   
}
void pwd(char **str)
{
    char *path;
    char s[100];
    path = getcwd(s,100);
    if(!path)
        printf("ereur");
    else
        printf("%s\n",getcwd(s,100));
} 
void env(char **str , s_env *head)
{
    if(str[1] != NULL)
    {
        printf("No such file or directory\n");
        return;
    }
    while(head)
    {
        printf("%s/n",head->next)
        head = head->next;
    }
    
}


void execution_cas(char **args)
{
    if(strcmp(args[0] == "echo"))
        echo();
    if(strcmp(args[0] == "cd"))
        cd();
    if(strcmp(args[0] == "pwd"))
        pwd();
}

struct s_env
{
    char *env_var;
    struct s_env *next;
} t_env;

void addenv(char **env)
{
    if(!env || *! env)
        return(NULL);
    int i = 0;
    t_env *head = NULL;
    node  = malloc(sizeof(t_env));
    if(!node)
        return;
    while(env[i])
    {
        node -> env_var = env[i];
        node -> next = NULL;
        if(!head)
            head = node;
        else
        {
            t_env *tmp = head
            while(temp->next)
                temp = temp -> next;
            temp -> next = node
        }
        i++;
    }
}