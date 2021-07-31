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

typedef struct s_list_histoy
{
	struct s_list_histoy	*prev;
	char					*command;
	struct s_list_histoy	*next;
}			t_list_histoy;

typedef struct s_terminal
{
	char			*line;
	int				fd_history;
	t_list_histoy	*history_cmd;
}			t_terminal;

void		free_history(t_terminal *term);			//terminal_utils.c
void		ft_exit(t_terminal *term);				//terminal_utils.c
void		ft_print_n();							//terminal_utils.c
void		ft_add_history(t_terminal *term);		//terminal_history.c
void		read_file_history(t_terminal *term);	//terminal_history.c
void		save_history(t_terminal *term);			//terminal_history.c
#endif
