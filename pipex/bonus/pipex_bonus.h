/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 20:30:37 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/08 18:15:16 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h> 
# include <sys/wait.h>  
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 120
# endif

char	**ft_split(char const *s, char sep);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
char	**ft_free(char **arr, size_t n);
char	*ft_strdup(const char *s1);
void	clear_split(char **strs);
void	close_and_exit_error(int *p);
void	close_and_exit_success(int p);
void	ft_error(char *str);
char	*search_path(char **env);
char	*is_valid_command_name(char *cmd, char **paths);
int		check_in_directory(char	*cmd, char **cmds);
int		is_valid_absolute_path(char *argv, char **str);
char	*build_command(char *path, char **cmds, char *argv_cmd);
void	execute_program(char *argv_cmd, char *path, char **env);
int		first_children(char **argv, char *path, char **env);
int		create_childrens(int last_p, char *cmd, char *path, char **env);
void	last_child(char **argv, char *path, int last_p, char **env);
char	*search_path(char **env);
int		check_in_directory(char	*cmd, char **cmds);
int		handle_here_doc(char **argv, int *flag, int argc);
void	child_here_doc(int *p, char *limiter);
void	last_child_here_doc(char **argv, char *path, int last_p, char **env);
char	*ft_strchr(const char *string, int searchedChar);
char	*get_next_line(int fd);
void	ft_error2(char *str, int p);
void	ft_error_sys(char *str);
void	prepare_child(int input, int output);
#endif