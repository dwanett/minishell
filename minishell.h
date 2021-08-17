/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 23:57:24 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/17 17:43:30 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <term.h>
# include <signal.h>
# include <errno.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

# define STDERROR 2
# define STDOUT 1
# define STDIN 0

typedef struct s_list_histoy
{
	struct s_list_histoy	*prev;
	char					*command;
	struct s_list_histoy	*next;
}			t_list_histoy;

typedef struct s_list_env
{
	char				*name;
	char				*line;
	char				*update_variable;
	int					tmp_variable;
	struct s_list_env	*next;
}			t_list_env;

typedef struct s_fd
{
	int	history;
	int	error;
	int	out;
	int	in;
}				t_fd;

typedef struct s_flags
{
	int		export;
	int		def_com;
	int		error;
}				t_flags;

typedef struct s_info_command
{
	char					**command;
	int						is_def_command;
	int						number_command;
	t_fd					fd;
	struct s_info_command	*next;
}				t_info_command;

typedef struct s_terminal
{
	char			*line;
	t_list_env		*env;
	t_list_histoy	*history_cmd;
	t_fd			fd;
	char			*not_def_command[4];
	t_list_env		*update;
	t_list_env		*status;
	t_list_env		*path;
	t_flags			flag;
	char			**start_env;
}			t_terminal;

void		rl_replace_line (const char *, int);
void		free_history(t_terminal *term);					//terminal_utils.c
void		ft_exit(t_terminal *term);						//terminal_utils.c
void		ft_print_n();									//terminal_utils.c
void		ft_add_history(t_terminal *term);				//terminal_history.c
void		read_file_history(t_terminal *term);			//terminal_history.c
void		save_history(t_terminal *term);					//terminal_history.c
void		ft_export(char ***command, t_terminal *term, int size_arg);	//env.c
void		ft_unset(char ***command, t_terminal *term, int size_arg);	//env.c
void		ft_env(t_terminal *term, int flag, char ***command);		//env.c
void		update_variable_env(t_terminal *term, char *path_com,
				char *last_arg, t_list_env *tmp);						//env.c
int			pre_pars(t_terminal *term, char ****command_pipe,
				t_info_command **command_cur);							//pars.c
int			par_std_out(t_terminal *term, char **tmp,
				t_info_command **command_cur, t_info_command **last_elem);//pars.c
int			count_symbol_str(const char *str, char c);						//minishell.c
void		print_error(char *msg, char *error, int flag, t_terminal *term);//errors.c
int			check_not_def_com(char *line, char **not_def_com);//check_command.c
int			check_def_com(t_terminal *term, char *command, char **path);//check_command.c
void		init_t_teminal(t_terminal *term,
				int argc, char **argv, char **envp);					//init.c
int			init_term_fd(t_terminal *term);								//init.c
void		get_info_str_command(t_info_command **command_cur,
				t_terminal *term, char ***command_pipe, int ret);//get_info_str.c
int			check_def_command(char ***command, t_terminal *term);//def_command.c
void		pars_command(t_terminal *term,
				t_info_command **command_cur, int ret);
void		ft_export(char ***command, t_terminal *term, int size_arg); // export.c
int			is_name(char *elem, int flag);	//unset.c
void		del_element_env(char *elem, t_terminal *term);//unset.c
void		ft_unset(char ***command, t_terminal *term, int size_arg); //unset.c
int			is_ravenstvo(char *peremen);//ustils_export.c
int			is_new_perem_export(char *peremen, t_list_env *env);//ustils_export.c
#endif
