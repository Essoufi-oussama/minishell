#include "minishell.h"

int	main()
{
	char *line;
	t_token	**tokens;
	// int token_size;

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
		if (check_quotes(line) != 1)
		{
			tokens = tokenize(line);
			check_tokens(tokens);
			for(int i = 0; tokens[i]; i++)
			{
				printf("%s\n",tokens[i]->content);
				free(tokens[i]->content);
				free(tokens[i]);
			}
			free(tokens);
		}
		free(line);
	}
}