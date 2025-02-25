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
            env_vars[i++] = current->env_var;
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

void export2(char **str, t_data *data)
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
////

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
void execution_cas(t_command *cmd, t_env *env_list, t_data *data)
{
	char	**args;
    char **envs = convert_env_list_to_array(env_list, data);

	args = data->commands[0];
    if (ft_strcmp(args[0], "cd") == 0)
        cd(args);
    else if (ft_strcmp(args[0], "exit") == 0)
        exit_program(0);
	else if (ft_strcmp(args[0], "export") == 0)
	{
        export2(args, data);
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
        execution_cas(cmd, data->env, data);
    }
    else 
        pipe_cas(data->commands,data->env,data);
    return 0;
}

