#include "minishell.h"

int	main()
{
	char *line;
	int	*tokens;

	while(1)
	{
		line = readline("-> minishell ");
		if (!line)
		{
			printf("exit\n");
			exit(1);
		}
		if (strlen(line) > 0)
			add_history(line);
		tokens = tokenize(line);
		printf("%s\n", line);
			int	i = 0;

		while (tokens[i] != -1)
		{
			printf("%d  ", tokens[i]);
			i++;
		}
		printf("\n");
		free(line);
	}
}