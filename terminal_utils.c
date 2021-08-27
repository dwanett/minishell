/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 15:26:50 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/23 15:20:25 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_history(t_terminal *term)
{
	if (term->history_cmd == NULL)
		return ;
	while (term->history_cmd->next != NULL)
	{
		term->history_cmd = term->history_cmd->next;
		free(term->history_cmd->prev->command);
		free(term->history_cmd->prev);
	}
	free(term->history_cmd->command);
	free(term->history_cmd);
}

void	free_env(t_list_env *env)
{
	t_list_env	*tmp;

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

void	ft_exit_utils(t_terminal *term, long int *exot)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (term->line != NULL && (term->line)[i] == ' ')
		i++;
	while (term->line != NULL && (term->line)[i] != '\0')
	{
		if ((term->line)[i] == ' ' && *exot == 0)
		{
			while ((term->line)[i] == ' ')
				i++;
			j = i;
			if ((term->line)[i] == '-')
				j++;
			ft_exit_help(exot, j, i, term);
		}
		i++;
	}
}

// выход из терминала и сохранение истории
void	ft_exit(t_terminal *term)
{
	long int	exot;

	exot = 0;
	ft_putstr_fd("exit\n", 1);
	ft_exit_utils(term, &exot);
	if (exot != -2)
	{
		if (term->line && (term->history_cmd
				&& ft_strcmp(term->line, term->history_cmd->command)))
			ft_add_history(term);
		save_history(term);
		free_env(term->env);
		if (term->line != NULL)
			free(term->line);
		free_env_for_next_process(term->start_env);
		exit(exot);
	}
}

// CTRL C
void	ft_print_n(int a)
{
	(void)a;
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
