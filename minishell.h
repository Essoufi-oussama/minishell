/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 12:09:41 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/19 13:01:57 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef enum e_tokens_types
{
	PIPE ,
	INPUT_DIRECTION ,
	OUTPUT_DIRECTION ,
	OUT_APPEND ,
	HERE_DOC ,
	WORD ,
}	t_tokens_types;

typedef struct s_token
{
	int		type;
	int		quoted;
	int		expandable;
	int		part_of_previous;
	char	*content;
}	t_token;

typedef enum e_quote
{
	NO_QUOTE,
	S_QUOTE,
	D_QUOTE,
}	t_quote;

typedef	struct s_alloc
{
	void	*addr;
	struct s_alloc	*next;
}	t_alloc;

typedef struct s_redir
{
	char	*name;
	int		type;
	int		here_doc_expandable;
	struct s_redir *next;
}	t_redir;


typedef	struct s_command
{
	char	**args;
	int 	args_count;
	t_redir	*infiles;
	t_redir *outfiles;
}	t_command;

typedef	struct s_data
{
	char	*line;
	char	**env;
	int	exit_status;
	t_command	**commands;
	int		command_count;
	t_token		**tokens;
	t_alloc	*alloc;
}	t_data;

int		count_tokens(char *str);
int		lexing(t_token **tokens);
int		check_quotes(char *str);
int		is_operation(int i);
int		check_special_char(t_token **tokens);
int		is_word_char(char c);
int		is_special(char c);
int		to_handle(char c);
int		handle_quotes(char *str, int *i, char quote_char);
int		count_no_quotes(char *str);
int		check_consecutive_expressions(t_token **tokens);
int		check_end_and_pipe(t_token **tokens);

void	tokenize(t_data *data);
void	loop_token_arr(char *str, t_token **token_arr, t_data *data);
void	insert_other_ops(t_token *token, char *str, t_data *data);
t_token	*insert_token_arr_word(int *i, char *str, int quote, t_data *data);
t_token	*insert_token_arr_op(int *i, char *str, t_data *data);

void	expanding(t_data *data);
char	*handle_multiple_dollars(char	*token, int count, int i, t_data *data);
char	*build_exp_str(char *token, char *previous, int i, t_data *data);
char	*handle_quoted_token(char *token, t_data *data);
char	*expand_token(char *token, t_data *data);
char	*create_spaces(int count, t_data *data);
char	*join_tokens(char **strs, char *token, int j, t_data *data);
char	*get_expanded_value(char *token, int i, int *j, t_data *data);
int	skip_here_doc_limiter(t_token **tokens, int current);

void	parse(t_data *data);
int	count_commands(t_token **tokens);


size_t	ft_strlcpy(char *dest, const char *src, size_t dstsize);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s1, t_data *data);
int		ft_isprint(int c);
char	*ft_strjoin(char const *s1, char const *s2, t_data *data);
char	**ft_split(char const *s, char sep, t_data *data);
int		is_quote(char c);
char	*ft_itoa(int n, t_data *data);

void	ft_lstclear_garbage(t_alloc **lst);
void	ft_lstadd_front(t_alloc **lst, t_alloc *new);
void	*ft_malloc(size_t size, t_data *data);
#endif