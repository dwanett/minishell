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

void ft_exit(t_terminal *term) // выход из терминала и сохраниние истории
{
	save_history(term);
	free_history(term);
	ft_putstr_fd("exit\n", 1);
	exit(0);
}

void ft_print_n() // CTRL C
{
	ft_putstr_fd("\nminishell$ ", 1);
}