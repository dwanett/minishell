/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 23:57:24 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/17 19:26:48 by gparsnip         ###   ########.fr       */
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

typedef struct s_start_end
{
	int				i;
	int				start;
	int				end;
	char			*new_tmp;
	char			*fre;
}				t_start_end;

typedef struct s_input_or_output
{
	int		count;
	char	*start_name_file;
	char	*name;
	int		*fd;
	char	c;
}				t_input_or_output;

typedef struct s_pars_env_elem
{
	int		i;
	char	*tmp;
	char	*tmp_2;
	int		open_one;
	int		open_two;
	int		size_name;
}				t_pars_env_elem;

typedef struct s_char_record
{
	int	i;
	int	j;
	int	g;
}				t_char_record;
typedef struct s_pars_def_command
{
	pid_t	pid;
	int		l;
	int		status;
}				t_pars_def_command;

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

void		rl_replace_line(const char *lol, int kek);
void		free_history(t_terminal *term);					//terminal_utils.c
void		ft_exit(t_terminal *term);						//terminal_utils.c
void		ft_print_n(int a);								//terminal_utils.c
void		ft_add_history(t_terminal *term);				//terminal_history.c
void		read_file_history(t_terminal *term);			//terminal_history.c
void		save_history(t_terminal *term);					//terminal_history.c
void		ft_export(char ***command, t_terminal *term, int size_arg);	//env.c
void		ft_unset(char ***command, t_terminal *term, int size_arg);	//env.c
void		ft_env(t_terminal *term, int flag, char ***command);		//env.c
void		update_variable_env(t_terminal *term, char *path_com,
				char *last_arg, t_list_env *tmp);						//env.c
int			pre_pars(t_terminal *term, char ****command_pipe,
				t_info_command **command_cur, int i);
int			par_std_out(t_terminal *term, char **tmp,
				t_info_command **command_cur, t_info_command **last_elem);
int			count_symbol_str(const char *str, char c);
void		print_error(char *msg, char *error, int flag, t_terminal *term);
int			check_not_def_com(char *line, char **not_def_com);
int			check_def_com(t_terminal *term, char *command, char **path);
void		init_t_teminal(t_terminal *term,
				int argc, char **argv, char **envp);
int			init_term_fd(t_terminal *term);
void		get_info_str_command(t_info_command **command_cur,
				t_terminal *term, char ***command_pipe, int ret);
int			check_def_command(char ***command, t_terminal *term);
void		pars_command(t_terminal *term,
				t_info_command **command_cur, int ret);
void		ft_export(char ***command, t_terminal *term, int size_arg);
int			is_name(char *elem, int flag);
void		del_element_env(char *elem, t_terminal *term);
void		ft_unset(char ***command, t_terminal *term, int size_arg);
void		par_multi_cammand(t_terminal *term);
int			is_ravenstvo(char *peremen);
int			is_new_perem_export(char *peremen, t_list_env *env);
int			ft_size_matrix_and_trim(char **matrix,
				char *c, t_terminal *term, char ****command_pipe);
void		is_input_or_output_init(t_input_or_output *all,
				t_terminal *term, char *tmp, int *i);
void		command_pipe_null(char ****command_pipe, int i, int *ret);
void		pars_env_elem(t_terminal *term, char **command_cur);
char		*get_name_file_and_fd(t_terminal *term,
				char *start_name_file, char *tmp, int *i);
int			pars_quotes(char **command, t_terminal *term,
				char ****command_pipe, int i);
int			all_name_null(t_terminal *term, t_input_or_output *all);
void		count_bad(char *command, int *i);
int			ligic_quost_two(char *command);
#endif
