/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 12:09:41 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/16 18:23:18 by oessoufi         ###   ########.fr       */
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


int		count_tokens(char *str);
void	check_tokens(t_token **tokens);
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

t_token	**tokenize(char *str, t_alloc **head);
void	loop_token_arr(char *str, t_token **token_arr, t_alloc **head);
void	insert_other_ops(t_token *token, char *str, t_alloc **head);
t_token	*insert_token_arr_word(int *i, char *str, int quote, t_alloc **head);
t_token	*insert_token_arr_op(int *i, char *str, t_alloc **head);

void	expanding(t_token **tokens, t_alloc **head);
char	*build_exp_str(char *token, char *previous, int i, t_alloc **head);
char	*handle_quoted_token(char *token, t_alloc **head);
char	*expand_token(char *token, t_alloc **head);
char	*create_spaces(int count, t_alloc **head);
char	*join_tokens(char **strs, char *token, int j, t_alloc **head);
char	*get_expanded_value(char *token, int i, int *j, t_alloc **head);

size_t	ft_strlcpy(char *dest, const char *src, size_t dstsize);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s1, t_alloc **head);
int		ft_isprint(int c);
char	*ft_strjoin(char const *s1, char const *s2, t_alloc **head);
char	**ft_split(char const *s, char sep, t_alloc **head);

void	ft_lstclear_garbage(t_alloc **lst);
void	ft_lstadd_front(t_alloc **lst, t_alloc *new);
void	*ft_malloc(size_t size, t_alloc **head);
#endif