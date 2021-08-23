/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_multi_command_help.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 15:39:11 by gparsnip          #+#    #+#             */
/*   Updated: 2021/08/23 16:18:29 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	par_std_out_init(t_start_end *all,
		t_info_command **tmp_com, t_terminal *term)
{
	(*all).i = 0;
	(*all).open_1 = 0;
	(*all).open_2 = 0;
	(*all).new_tmp = NULL;
	(*tmp_com) = (t_info_command *)malloc(sizeof(t_info_command));
	if ((*tmp_com) == NULL)
		print_error(NULL, strerror(errno), 0, term);
	(*tmp_com)->next = NULL;
	(*tmp_com)->command = NULL;
}

void	qoutes_help_pars_std(t_start_end *all, char **tmp)
{
	if ((*all).open_1 == 0 && (*all).open_2 == 0 && (*tmp)[(*all).i] == '\'')
		(*all).open_1 = 1;
	else if ((*all).open_2 == 0 && (*tmp)[(*all).i] == '\'')
		(*all).open_1 = 0;
	if ((*all).open_2 == 0 && (*all).open_1 == 0 && (*tmp)[(*all).i] == '"')
		(*all).open_2 = 1;
	else if ((*all).open_1 == 0 && (*tmp)[(*all).i] == '"')
		(*all).open_2 = 0;
}

int	par_std_out(t_terminal *term, char **tmp,
	t_info_command **command_cur, t_info_command **last_elem)
{
	t_start_end		all;
	t_info_command	*tmp_com;

	par_std_out_init(&all, &tmp_com, term);
	while ((*tmp)[all.i] != '\0')
	{
		qoutes_help_pars_std(&all, tmp);
		if (all.open_1 == 0 && all.open_2 == 0
			&& ((*tmp)[all.i] == '>' || (*tmp)[all.i] == '<'))
		{
			if (par_std_out_utils(&all, tmp, term) == 1)
			{
				init_tmp_com(&tmp_com, term, last_elem, command_cur);
				return (1);
			}
			free(all.fre);
			free((*tmp));
			(*tmp) = ft_strdup(all.new_tmp);
			free(all.new_tmp);
			all.new_tmp = NULL;
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
