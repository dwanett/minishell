/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_multi_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 18:14:47 by gparsnip          #+#    #+#             */
/*   Updated: 2021/08/17 18:39:40 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_standart_input(t_input_or_output info)
{
	char *str;
	t_pars_def_command all;

	str = NULL;
	all.pid = fork();
	if (all.pid == 0)
	{
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
	waitpid(all.pid, &(all.status), 0);
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

int	par_std_out(t_terminal *term, char **tmp,
	t_info_command **command_cur, t_info_command **last_elem)
{
	t_start_end		all;
	t_info_command	*tmp_com;

	all.i = 0;
	all.new_tmp = NULL;
	tmp_com = (t_info_command *)malloc(sizeof(t_info_command));
	if (tmp_com == NULL)
		print_error(NULL, strerror(errno), 0, term);
	tmp_com->next = NULL;
	tmp_com->command = NULL;
	while ((*tmp)[all.i] != '\0')
	{
		if ((*tmp)[all.i] == '>' || (*tmp)[all.i] == '<')
		{
			if (par_std_out_utils(&all, tmp, term) == 1)
			{
				init_tmp_com(&tmp_com, term, last_elem, command_cur); //Влад бля (забыл)
				return (1);
			}
			free(all.fre);
			free((*tmp));
			(*tmp) = ft_strdup(all.new_tmp);
			free(all.new_tmp);
			all.i = 0;
		}
		all.i++;
	}
	init_tmp_com(&tmp_com, term, last_elem, command_cur);
	return (0);
}

void	par_multi_cammand(t_terminal *term)
{
	int		pos;
	char	*tmp;

	if (count_symbol_str(term->line, ';') != 0)
	{
		pos = ft_strclen(term->line, ';');
		tmp = ft_strndup(term->line, pos);
		free(term->line);
		term->line = tmp;
		term->flag.error = 1;
	}
}
