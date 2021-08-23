/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   def_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 17:06:56 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/23 15:23:50 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit_help(int *exot, int j, int i, t_terminal *term)
{
	while (ft_isdigit((term->line)[j]))
	{
		*exot = *exot * 10 + (term->line)[j] - '0';
		j++;
	}
	if ((term->line)[j] != '\0')
	{
		ft_putstr_fd("minishell: exit: ", 1);
		while ((term->line)[i] != '\0')
		{
			ft_putchar_fd((term->line)[i], 1);
			i++;
		}
		ft_putstr_fd(": numeric argument required\n", 1);
		*exot = -1;
	}
	if ((term->line)[i] == '-')
		*exot = -1;
}

int	is_path(const char *command)
{
	int	i;

	i = 0;
	while (command[i] != '\0')
	{
		if (command[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

int	check_def_command(char ***command, t_terminal *term)
{
	char	*path;
	char	*tmp;

	if (!is_path(**command))
	{
		if (check_def_com(term, **command, &path))
		{
			if ((**command)[0] != '\0')
				print_error(*command[0], "command not found", -1, term);
			return (0);
		}
		tmp = ft_strjoin(path, "/");
		free(path);
		path = tmp;
		tmp = ft_strjoin(path, **command);
		free(**command);
		**command = tmp;
		free(path);
	}
	return (1);
}
