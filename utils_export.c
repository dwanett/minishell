/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 17:42:05 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/17 17:42:57 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Проверка на дубликат переменной
int	is_new_perem_export(char *peremen, t_list_env *env)
{
	t_list_env	*tmp;

	tmp = env;
	while (tmp != NULL)
	{
		if (!ft_strncmp(tmp->name, peremen, ft_strclen(peremen, '=')))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	is_ravenstvo(char *peremen)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (peremen[i] != '\0')
	{
		if (peremen[i] == '=')
			j = 1;
		i++;
	}
	return (j);
}

void	ft_env_print_export(t_list_env *tmp, t_terminal *term)
{
	ft_putstr_fd("declare -x ", term->fd.out);
	ft_putstr_fd(tmp->name, term->fd.out);
	if (tmp->is_initialized == 1)
	{
		ft_putstr_fd("=\"", term->fd.out);
		ft_putstr_fd(tmp->line, term->fd.out);
		ft_putstr_fd("\"", term->fd.out);
	}
	ft_putstr_fd("\n", term->fd.out);
}
