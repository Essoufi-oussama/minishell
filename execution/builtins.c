/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 19:49:21 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/02/18 19:51:24 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"
// {"echo", "-n" , "hello", "world",NULL};
// {"echo", "hello","world",NULL};

void echo(char **str)
{
    int i = 1;
    int option_flag = 0;
    int j = 0;
    if(strcmp(str[1],"-n") == 0)
    {
        option_flag = 1;
        i = 2;
    }
    while(str[i] == "-n")
        i++;
    while(str[i])
    {
        j = 0;
        while(str[i][j])
        {
            write(1,&str[i][j],1);
            j++;
        }
        if(str[i + 1] != NULL)
            write(1," ",1);
        i++;
    }
    if(option_flag == 0)
        write(1,"\n",1);
}
void cd(char **str)
{
    if(chdir(str[1]) != 0)
        perror("No such file or directory");
    
}
