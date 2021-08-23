/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_multi_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 18:14:47 by gparsnip          #+#    #+#             */
/*   Updated: 2021/08/23 15:39:45 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_ign(int a)
{
	(void)a;
	printf("\n");
}

void	read_standart_input(t_input_or_output info)
{
	char				*str;
	t_pars_def_command	all;

	str = NULL;
	all.pid = fork();
	if (all.pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		while (str == NULL || ft_strcmp(str, info.name))
		{
			if (str != NULL)
				free(str);
			str = readline("> ");
			if (str == NULL)
				break ;
		}
		free(str);
		exit(0);
	}
	signal(SIGINT, print_ign);
	waitpid(all.pid, &(all.status), 0);
	signal(SIGINT, ft_print_n);
	free(info.name);
}

int	is_input_or_output(t_terminal *term, char *tmp, int *i)
{
	t_input_or_output	all;

	is_input_or_output_init(&all, term, tmp, i);
	while (tmp[*i] == '>' || tmp[*i] == '<' || tmp[*i] == ' ')
	{
		if (tmp[*i] == '>' || tmp[*i] == '<')
			all.count++;
		if ((tmp[*i] == '<' && all.count > 2)
			|| (tmp[*i] == '>' && all.count > 2))
		{
			ft_putstr_fd(tmp + *i - all.count, term->fd.error);
			ft_putstr_fd(": error syntax\n", term->fd.error);
			return (1);
		}
		(*i)++;
	}
	all.start_name_file = tmp + *i;
	all.name = get_name_file_and_fd(term, all.start_name_file, tmp, i);
	if (all.name != NULL)
		if (all_name_null(term, &all) == 1)
			return (1);
	if (all.count == 2 && all.c == '<')
		read_standart_input(all);
	return (0);
}

void	init_tmp_com(t_info_command	**tmp_com, t_terminal *term,
	t_info_command **last_elem, t_info_command **command_cur)
{
	(*tmp_com)->fd.in = term->fd.in;
	(*tmp_com)->fd.out = term->fd.out;
	(*tmp_com)->fd.error = term->fd.error;
	(*tmp_com)->fd.history = term->fd.history;
	term->fd.in = STDIN;
	term->fd.out = STDOUT;
	term->fd.error = STDERROR;
	if ((*command_cur) == NULL)
	{
		(*command_cur) = (*tmp_com);
		(*last_elem) = (*command_cur);
	}
	else
	{
		(*last_elem)->next = (*tmp_com);
		(*last_elem) = (*last_elem)->next;
	}
}

int	par_std_out_utils(t_start_end *all, char **tmp, t_terminal *term)
{
	(*all).start = (*all).i;
	while ((*tmp)[(*all).start - 1] == ' ')
		(*all).start--;
	if (is_input_or_output(term, *tmp, &((*all).i)))
		return (1);
	(*all).end = (*all).i;
	while ((*tmp)[(*all).end] == ' ')
		(*all).end++;
	if ((*all).new_tmp != NULL)
		free((*all).new_tmp);
	(*all).new_tmp = ft_strndup(*tmp, (*all).start);
	(*all).fre = (*all).new_tmp;
	(*all).new_tmp = ft_strjoin((*all).new_tmp, (*tmp) + (*all).end);
	return (0);
}
