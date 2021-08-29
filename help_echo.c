/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/29 03:00:28 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/29 03:00:28 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	size_two_mass(char **mass)
{
	int	i;

	i = 0;
	while (mass[i] != NULL)
		i++;
	return (i);
}

void	replace_repeat_flags_help(char ***command_pipe,
		char ***new_command, int n_last, int count)
{
	int	j;
	int	i;

	j = 1;
	i = 1;
	if (n_last > 0)
	{
		(*new_command)[j] = ft_strdup("-n");
		j++;
	}
	free((*command_pipe)[0]);
	while ((*command_pipe)[i] != NULL)
	{
		if (i > count)
		{
			(*new_command)[j] = ft_strdup((*command_pipe)[i]);
			j++;
		}
		free((*command_pipe)[i]);
		i++;
	}
	(*new_command)[j] = NULL;
}

void	replace_repeat_flags(char ***command_pipe,
			 t_terminal *term, int count, int n_last)
{
	int		size;
	char	**new_command;

	if (count < n_last)
		count = n_last;
	size = size_two_mass((*command_pipe));
	if (n_last > 0)
		size++;
	new_command = (char **)malloc(sizeof(char *) * (size - count + 1));
	if (new_command == NULL)
		print_error(NULL, strerror(errno), 0, term);
	new_command[0] = ft_strdup((*command_pipe)[0]);
	replace_repeat_flags_help(command_pipe, &new_command, n_last, count);
	free((*command_pipe));
	(*command_pipe) = new_command;
}

void	check_repeat_flags_help(char ***command_pipe, t_repeat_flags *all)
{
	while ((*command_pipe)[(*all).i][0] == '-'
		&& (*command_pipe)[(*all).i][(*all).j] != '\0')
	{
		if (((*command_pipe)[(*all).i][(*all).j] == 'n'
			|| (*command_pipe)[(*all).i][(*all).j] == 'E'
			|| (*command_pipe)[(*all).i][(*all).j] == 'e')
			&& (*all).k != (*all).i)
		{
			(*all).count++;
			(*all).k = (*all).i;
		}
		if ((*command_pipe)[(*all).i][(*all).j] == 'n'
			&& (*all).flag != (*all).i)
			(*all).flag = (*all).i;
		if ((*command_pipe)[(*all).i][(*all).j] != 'n'
			&& (*command_pipe)[(*all).i][(*all).j] != 'E'
				&& (*command_pipe)[(*all).i][(*all).j] != 'e')
		{
			(*all).flag = (*all).n_last;
			(*all).count--;
		}
		(*all).j++;
	}
}

void	check_repeat_flags(char ***command_pipe, t_terminal *term)
{
	t_repeat_flags	all;

	all.i = 1;
	all.count = 0;
	all.n_last = 0;
	all.flag = -1;
	all.k = 0;
	while ((*command_pipe)[all.i] != NULL && (*command_pipe)[all.i][0] == '-')
	{
		all.j = 1;
		check_repeat_flags_help(command_pipe, &all);
		if (all.flag == all.n_last)
			break ;
		all.n_last = all.flag;
		all.i++;
	}
	if (all.n_last == 0 && all.count == 0)
		return ;
	replace_repeat_flags(command_pipe, term, all.count, all.n_last);
}
