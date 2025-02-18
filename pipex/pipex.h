/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 11:54:41 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/08 18:08:44 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>

char	**ft_split(char const *s, char sep);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
char	**ft_free(char **arr, size_t n);
char	*ft_strdup(const char *s1);
void	clear_split(char **strs);
void	close_and_exit_error(int *p);
void	close_and_exit_success(int *p, pid_t *pid, pid_t *pid2);
void	ft_error(char *str);
char	*search_path(char **env);
char	*is_valid_command_name(char *cmd, char **paths);
int		is_valid_absolute_path(char *argv, char **str);
int		check_in_directory(char	*cmd, char **cmds);
char	*build_command(char *path, char **cmds, char *argv_cmd);
void	execute_program(char *argv_cmd, char *path, char **env);
void	child_process2(char **argv, char *path, int *p, char **env);
void	child_process(char **argv, char *path, int *p, char **env);
#endif
