/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 23:57:24 by dwanetta          #+#    #+#             */
/*   Updated: 2021/07/30 20:06:08 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <term.h>
#include <signal.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define STDERROR 2
#define STDOUT 1
#define STDIN 0

typedef struct s_list_histoy
{
	struct s_list_histoy	*prev;
	char					*command;
	struct s_list_histoy	*next;
}			t_list_histoy;

typedef struct s_list_env
{
	char		*name;
	char		*line;
	char		*update_variable;
	struct s_list_env *next;
}			t_list_env;

typedef struct s_fd
{
	int error;
	int out;
	int in;
}				t_fd;

typedef struct s_terminal
{
	char			*line;
	t_list_env 		*env;
	int				fd_history;
	t_list_histoy	*history_cmd;
	t_fd			fd;
	char			*not_def_command[4];
	int				flag_export;
	int				flag_def_com;
	t_list_env		*update;
}			t_terminal;

void		free_history(t_terminal *term);											//terminal_utils.c
void		ft_exit(t_terminal *term);												//terminal_utils.c
void		ft_print_n();															//terminal_utils.c
void		ft_add_history(t_terminal *term);										//terminal_history.c
void		read_file_history(t_terminal *term);									//terminal_history.c
void		save_history(t_terminal *term);											//terminal_history.c
void		ft_export(char ***command, t_terminal *term, int size_arg);				//env.c
void		ft_unset(char ***command, t_terminal *term, int size_arg);				//env.c
void		ft_env(t_terminal *term, int flag);										//env.c
void		update_variable_env(t_terminal *term, char *path_com, char *last_arg);	//env.c
int			pre_pars(t_terminal *term, char ****command_pipe);						//pars.c
int			count_symbol_str(const char *str, char c);								//minishell.c
#endif
