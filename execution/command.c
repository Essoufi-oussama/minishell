/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:12:09 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/02/25 15:32:36 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int execu_cmd(char **str, char **env_list, t_data *data)
{
    int pid;
    char *path;
    pid = fork();
    if(pid < 0)
        return(1);
    if(pid == 0)
    {   
        infile_cas(data->commands[0]);
        outfile_cas(data->commands[0]);
        check_if_building(str,data->env,data);
        path = get_path(env_list,str[0], data);
        if(!path)
        {
            perror(str[0]);
            exit (0);
        }
        if (execve(path, str, env_list) == -1)
        {
            perror(str[0]);
            exit(1);
        }
    }
    else
        wait(NULL);
    return(0);
}
void execution_cas(char **args, t_env *env_list, t_data *data)
{
    char **envs = convert_env_list_to_array(env_list, data);
    if (ft_strcmp(args[0], "cd") == 0)
        cd(args);
    else if (ft_strcmp(args[0], "exit") == 0)
        exit_program(0);
	else if (ft_strcmp(args[0], "export") == 0)
	{
		export(args, data);
	}
    else
        execu_cmd(args, envs, data);
}

void addenv(char **env, t_env **head)
{
    if (!env || !*env)
        return;
    int i = 0;
    while (env[i])
    {
        t_env *node = malloc(sizeof(t_env));
        node->env_var = strdup(env[i]);
        node->next = NULL;
        if (!*head)
            *head = node;
        else {
            t_env *temp = *head;
            while (temp->next)
                temp = temp->next;
            temp->next = node;
        }
        i++;
    }
}

char **convert_env_list_to_array(t_env *head, t_data *data)
{
    int count = 0;
    t_env *temp = head;

    while (temp) {
        count++;
        temp = temp->next;
    }
    char **envs = ft_malloc((count + 1) * sizeof(char *), data);
    temp = head;
    int i = 0;
    while (temp) {
        envs[i] = temp->env_var;
        temp = temp->next;
        i++;
    }
    envs[i] = NULL;
    return envs;
}

int    execute(t_data *data)
{
    if(data->command_count == 1)
    {
        t_command *cmd = data->commands[0];
        execution_cas(cmd->args, data->env, data);
    }
    else 
        pipe_cas(data->commands,data->env,data);
    return 0;
}