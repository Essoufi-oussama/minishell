#include "minishell.h"

int	main()
{
	char *line;
	int	*tokens;
	int token_size;

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
		tokens = tokenize(line, &token_size);
		// for (int i = 0; tokens[i] != -1; i++)
		// 	printf("%d  ", tokens[i]);
		// printf("\n");
		check_tokens(tokens, token_size);
		free(line);
	}
}