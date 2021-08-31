/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/27 20:42:02 by gparsnip          #+#    #+#             */
/*   Updated: 2021/08/27 20:45:40 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_UTILS_H
# define MINISHELL_UTILS_H

# include "minishell.h"

typedef struct s_fd
{
	int	history;
	int	error;
	int	out;
	int	in;
}				t_fd;

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
	int				open_1;
	int				open_2;
	t_info_command	*tmp_com;
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
	int		i;
	int		status;
}				t_pars_def_command;

typedef struct s_repeat_flags
{
	int		count;
	int		i;
	int		j;
	int		flag;
	int		n_last;
	int		k;
}				t_repeat_flags;

#endif
