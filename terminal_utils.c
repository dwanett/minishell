/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 15:26:50 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/13 19:38:56 by gparsnip         ###   ########.fr       */
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
		free(tmp->name);
		free(tmp->line);
		if (tmp->update_variable != NULL)
			free(tmp->update_variable);
		free(tmp);
	}
}

void ft_exit_utils(t_terminal *term)
{
	int	i;

	i = 0;
	while (term->line != NULL && (term->line)[i] == ' ')
				i++;
	while (term->line != NULL && (term->line)[i] != '\0')
	{
		if ((term->line)[i] == ' ')
		{
			while ((term->line)[i] == ' ')
				i++;
			if ((term->line)[i] != '\0')
			{
				ft_putstr_fd("minishell: exit: ", 1);
				while ((term->line)[i] != '\0')
				{
					ft_putchar_fd((term->line)[i], 1);
					i++;
				}
				ft_putstr_fd(": numeric argument required\n", 1);
			}
		}
		i++;
	}
}

void ft_exit(t_terminal *term) // выход из терминала и сохранение истории
{
	if (term->line && (term->history_cmd && ft_strcmp(term->line, term->history_cmd->command)))
		ft_add_history(term);
	save_history(term);
	free_env(term->env);
	ft_putstr_fd("exit\n", 1);
	ft_exit_utils(term);
	if (term->line != NULL)
		free(term->line);
	//free_history(term);
	//ft_putstr_fd("exit\n", 1);
	exit(0);
}

void ft_print_n() // CTRL C
{
	rl_on_new_line();
	rl_redisplay();
	tputs(cursor_right, 1, ft_putchar);
	tputs(delete_character, 1, ft_putchar);
	tputs(cursor_left, 1, ft_putchar);
	tputs(delete_character, 1, ft_putchar);
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
