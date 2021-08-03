/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 15:26:50 by dwanetta          #+#    #+#             */
/*   Updated: 2021/07/31 15:26:50 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_history(t_terminal *term)
{
	if (term->history_cmd == NULL)
		return;
	while (term->history_cmd->next != NULL)
	{
		term->history_cmd = term->history_cmd->next;
		free(term->history_cmd->prev->command);
		free(term->history_cmd->prev);
	}
	free(term->history_cmd->command);
	free(term->history_cmd);
}

void free_env(t_list_env *env)
{
	t_list_env *tmp;

	while (env != NULL)
	{
		tmp = env;
		env = env->next;
		free(tmp->line);
		free(tmp);
	}
}

void ft_exit(t_terminal *term) // выход из терминала и сохранение истории
{
	if (term->line && (term->history_cmd && ft_strcmp(term->line, term->history_cmd->command)))
		ft_add_history(term);
	save_history(term);
	free_env(term->env);
	//free_history(term);
	ft_putstr_fd("exit\n", 1);
	exit(0);
}

void ft_print_n() // CTRL C
{
	ft_putstr_fd("\nminishell$ ", 1);
}
