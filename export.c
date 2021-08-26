/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 17:29:28 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/23 20:08:35 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_export_help_2(char ***command,
			t_terminal *term, t_list_env **new_env, int i)
{
	while (term->env && term->env->next != NULL)
		term->env = term->env->next;
	(*new_env) = (t_list_env *)malloc(sizeof(t_list_env));
	if ((*new_env) == NULL)
		print_error(NULL, strerror(errno), 0, term);
	(*new_env)->name = ft_strndup(*(*command + i),
			 ft_strclen(*(*command + i), '='));
	if (!ft_strcmp((*new_env)->name, "PATH"))
		term->path = (*new_env);
	(*new_env)->line = ft_strdup(*(*command + i)
			+ ft_strclen(*(*command + i), '=') + 1);
	(*new_env)->update_variable = NULL;
	update_variable_env(term, NULL, (*new_env)->name, NULL);
	(*new_env)->tmp_variable = 0;
	(*new_env)->next = NULL;
}

int	ft_export_init_and_check(char ***command,
		t_terminal *term, int *i, t_list_env **tmp)
{
	*i = 1;
	*tmp = term->env;
	free(term->status->line);
	term->status->line = ft_strdup("0");
	if (!*(*command + 1))
	{
		ft_env(term, 1, NULL);
		return (1);
	}
	return (0);
}

void	ft_export_help_1(t_terminal *term,
			char ***command, t_list_env **tmp, int i)
{
	t_list_env	*new_env;

	if (is_new_perem_export(*(*command + i), *tmp))
		del_element_env(*(*command + i), term);
	ft_export_help_2(command, term, &new_env, i);
	if (term->env)
		term->env->next = new_env;
	else
		*tmp = new_env;
}

void	free_env_for_next_process(char **envp)
{
	int i;

	i = 0;
	while (envp[i] != NULL)
	{
		free(envp[i]);
		i++;
	}
}

void	ft_export(char ***command, t_terminal *term, int size_arg)
{
	t_list_env	*tmp;
	int			i;

	if (ft_export_init_and_check(command, term, &i, &tmp))
		return ;
	while (i != size_arg)
	{
		term->env = tmp;
		if (is_ravenstvo(*(*command + i)))
		{
			if (!is_name(*(*command + i), 1) && term->flag.export != 1)
				ft_export_help_1(term, command, &tmp, i);
			else
				print_error(*(*command + i), "not a valid identifier", 3, term);
		}
		i++;
	}
	term->env = tmp;
	free_env_for_next_process(term->start_env);
	init_env_for_next_process(term, term->env);
}
