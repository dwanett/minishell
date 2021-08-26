/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 15:14:14 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/19 15:42:54 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	all_name_null(t_terminal *term, t_input_or_output *all)
{
	if ((*all).c == '>')
	{
		*((*all).fd) = open((*all).name, O_CREAT | O_EXCL | O_RDWR, S_IRWXU);
		if (*((*all).fd) == -1)
		{
			if ((*all).count == 2)
				*((*all).fd) = open((*all).name, O_APPEND | O_RDWR);
			else
				*((*all).fd) = open((*all).name, O_TRUNC | O_RDWR);
		}
		free((*all).name);
	}
	else if (all->count != 2)
		*((*all).fd) = open((*all).name, O_RDWR);
	if (*((*all).fd) == -1)
	{
		print_error((*all).name, strerror(errno), -1, term);
		return (1);
	}
	return (0);
}

void	is_input_or_output_init(t_input_or_output *all,
	t_terminal *term, char *tmp, int *i)
{
	(*all).count = 0;
	if (term->fd.out != 0 && term->fd.out != -1
		&& term->fd.out != 1 && term->fd.out != 2)
		close(term->fd.out);
	if (term->fd.in != 0 && term->fd.in != -1
		&& term->fd.in != 1 && term->fd.in != 2)
		close(term->fd.in);
	if (tmp[*i] == '>')
		(*all).fd = &(term->fd.out);
	else
		(*all).fd = &(term->fd.in);
	(*all).c = tmp[*i];
}

void	pre_pars_utils(t_terminal *term, int i, char **tmp)
{
	if (!ft_strncmp(tmp[i], "export", 6))
	{
		term->flag.export = 2;
		if (count_symbol_str(tmp[i], '$') != 0)
			term->flag.export = 1;
	}
}

void	pre_pars_init(int *ret,
	t_terminal *term, t_info_command **command_cur, char ***tmp)
{
	(*ret) = 1;
	par_multi_cammand(term);
	(*command_cur) = NULL;
	(*tmp) = ft_split_pipe(term->line);
}

int	pre_pars(t_terminal *term,
	char ****command_pipe, t_info_command **command_cur, int i)
{
	char			**tmp;
	int				size;
	int				ret;
	t_info_command	*last_elem;

	pre_pars_init(&ret, term, command_cur, &tmp);
	size = ft_size_matrix_and_trim(tmp, " ", term, command_pipe);
	i = 0;
	while (i != size)
	{
		if (par_std_out(term, &tmp[i], command_cur, &last_elem))
			command_pipe_null(command_pipe, i, &ret);
		pre_pars_utils(term, i, tmp);
		pars_env_elem(term, &tmp[i]);
		if (pars_quotes(&tmp[i], term, command_pipe, i))
			command_pipe_null(command_pipe, i, &ret);
		free(tmp[i]);
		i++;
	}
	free(tmp);
	(*command_pipe)[i] = NULL;
	return (ret);
}
