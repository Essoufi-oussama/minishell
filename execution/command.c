#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int execu_cmd(char *str, char **env)
{
    char *path;
    int pid;
    char **args;

    path = get_path(env,str);
    if(!path)
        return (perror("Error"), 1);
    pid = fork();
    if(pid < 0)
        return(free(path1),1);
    if(pid == 0)
    {
        args = ft_split(argv[2], ' ');
	    if (!args)
		    return(free(path),1);
        if (execve(path, args, NULL) == -1)
	    {
		    free_ultimate(args);
            free(path);
		    perror("Error");
		    exit(1);
	    }
    }
    else
        wait(NULL);
    free(path);
    free_ultimate(args);
    return(0);
}
