/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 02:41:57 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/11 17:39:28 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	addenv(char **env, t_env **head)
{
	int		i;
	t_env	*last;
	t_env	*node;

	i = 0;
	last = NULL;
	while (env[i])
	{
		node = malloc(sizeof(t_env));
		if (!node)
			return (0);
		node->env_var = ftt_strdup(env[i]);
		if (!node->env_var)
			return (free(node), 0);
		node->next = NULL;
		if (!*head)
			*head = node;
		else
			last->next = node;
		last = node;
		i++;
	}
	
	return (1);
}

char	**convert_env_list_to_array(t_env *head, t_data *data)
{
	t_env	*temp;
	int		i;
	char	**envs;

	i = 0;
	temp = head;
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	envs = ft_malloc((i + 1) * sizeof(char *), data);
	i = 0;
	while (head)
	{
		envs[i] = ft_strdup(head->env_var, data);
		head = head->next;
		i++;
	}
	return (envs[i] = NULL, envs);
}
