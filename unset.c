/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 17:32:22 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/23 20:09:24 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	del_element_env_help(t_terminal *term,
		t_list_env **tmp, t_list_env **prev)
{
	(*prev)->next = (*tmp)->next;
	if (!ft_strcmp((*tmp)->name, "PATH"))
		term->path = NULL;
	free((*tmp)->name);
	free((*tmp)->line);
	if ((*tmp)->update_variable != NULL)
	{
		free((*tmp)->update_variable);
		free(term->update);
		term->update = NULL;
	}
	else
		free((*tmp));
	(*tmp) = (*prev);
}

//Удаление переменной
void	del_element_env(char *elem, t_terminal *term)
{
	t_list_env	*tmp;
	t_list_env	*prev;

	tmp = term->env;
	prev = tmp;
	while (tmp != NULL)
	{
		if (!ft_strncmp(tmp->name, elem, ft_strclen(elem, '=')))
		{
			if (prev == term->env)
				term->env = term->env->next;
			else
				del_element_env_help(term, &tmp, &prev);
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	is_name(char *elem, int flag)
{
	int	i;

	i = 0;
	while (elem[i] != '\0')
	{
		if (flag == 1 && elem[i] == '=' && i != 0)
			break ;
		if (!ft_isalpha(elem[i]) && elem[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

// не всегда удаляет переменную (причина неизвестна)
void	ft_unset(char ***command, t_terminal *term, int size_arg)
{
	int	i;

	i = 1;
	free(term->status->line);
	term->status->line = ft_strdup("0");
	while (i != size_arg)
	{
		if (!is_name(*(*command + i), 0))
			del_element_env(*(*command + i), term);
		else
			print_error(*(*command + i), "not a valid identifier", 2, term);
		i++;
	}
	free_env_for_next_process(term->start_env);
	init_env_for_next_process(term, term->env);
}
