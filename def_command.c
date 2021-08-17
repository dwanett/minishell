/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   def_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 17:06:56 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/17 17:07:18 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
