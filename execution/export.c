/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 03:09:01 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/02/25 15:30:27 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void swap(char **a, char **b)
{
    char *temp = *a;
    *a = *b;
    *b = temp;
}


void bubble_sort(char **arr, int n)
{
    int i = 0;
    while (i < n - 1)
    {
        int j = 0;
        while (j < n - i - 1) {
            if (strcmp(arr[j], arr[j + 1]) > 0) 
                swap(&arr[j], &arr[j + 1]);
            j++;
        }
        i++;
    }
}


char **sort_export(t_env *head)
{
    char **env_vars;
    t_env *current;
    int count;
    int i;
    
    i = 0;
    count = 0;
    current = head;
    while (current)
    {
        if (current->env_var[0] != '_' && current->env_var[1] != '=') 
            count++;
        current = current->next;
    }
    env_vars = malloc((count + 1) * sizeof(char *));
    current = head;
    i = 0;
    while (current)
    {
        if (current->env_var[0] != '_' && current->env_var[1] != '=') 
            env_vars[i++] = strdup(current->env_var);
        current = current->next;
    }
    env_vars[i] = NULL;
    bubble_sort(env_vars, count);
    return env_vars;
}


t_env	*ft_lstlast2(t_env *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back2(t_env **lst, t_env *new)
{
	t_env	*last;

	if (new == NULL || lst == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = ft_lstlast2(*lst);
	last->next = new;
}

void add_export(char *str, t_data *data)
{
    (void)data;
    t_env *new_node = malloc(sizeof(t_env));
    new_node->env_var = strdup(str);
    new_node->next = NULL;
    ft_lstadd_back2(&(data->env), new_node);
}

void export(char **str, t_data *data)
{
    char **env_vars;
    int i;

    i = 0;
    if (str[1] == NULL)
    {
        env_vars = sort_export(data->env);
        while (env_vars[i])
        {
            char *key = env_vars[i];
            char *value = strchr(key, '=');
            if (value)
            {
                *value = '\0';
                value++;
                printf("declare -x %s=\"%s\"\n", key, value);
            } 
            else 
                printf("declare -x %s\n", key);
            i++;
        }
    } 
    else {
		
        add_export(str[1], data);
	}
}
