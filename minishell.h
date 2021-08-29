/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 23:57:24 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/23 16:12:59 by gparsnip         ###   ########.fr       */
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
# include "minishell_utils.h"

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
	int					is_initialized;
	struct s_list_env	*next;
}			t_list_env;

typedef struct s_flags
{
	int		export;
	int		def_com;
	int		error;
}				t_flags;

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
	t_list_env		*home;
	t_flags			flag;
	pid_t			pip;
	char			**start_env;
}			t_terminal;

void		rl_replace_line(const char *lol, int kek);
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
char		**ft_split_pipe(char *line);
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
void		pars_command(t_terminal *term, t_info_command **command_cur,
				int ret, int *er);
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
void		print_ign(int a);
void		ft_exit_help(long int *exot, int j, int i, t_terminal *term);
void		par_std_out_init(t_start_end *all,
				t_info_command **tmp_com, t_terminal *term);
int			par_std_out(t_terminal *term, char **tmp,
				t_info_command **command_cur, t_info_command **last_elem);
void		par_multi_cammand(t_terminal *term);
int			par_std_out_utils(t_start_end *all, char **tmp, t_terminal *term);
void		init_tmp_com(t_info_command	**tmp_com, t_terminal *term,
				t_info_command **last_elem, t_info_command **command_cur);
void		free_env_for_next_process(char **envp);
void		init_env_for_next_process(t_terminal *term, t_list_env *envp);
void		init_env(t_list_env **env, char **envp, t_terminal *term, int i);
void		init_env_for_next_process_help(int i,
				t_list_env *envp, t_terminal *term);
void		ft_status(t_terminal *term, int status, int *er);
void		check_repeat_flags(char ***command_pipe, t_terminal *term);
#endif
