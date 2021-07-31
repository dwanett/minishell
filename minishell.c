/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 23:57:29 by dwanetta          #+#    #+#             */
/*   Updated: 2021/07/30 20:05:21 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void teminal(t_terminal *term) //чтение строк терминала
{
	term->line = readline("minishell$ ");
	if (term->line == NULL || !ft_strncmp(term->line, "exit", 4))
		ft_exit(term);
	else if (ft_strcmp(term->line, "\0")) // если строка не пустая
		{
			ft_add_history(term);
			add_history(term->line); //добавление истории для readline
		}
	//command(term->line);	//функция обработки команд
}

int	main(int argc, char **argv, char **envp)
{
	t_terminal term;

	signal(SIGTSTP, SIG_IGN);
	signal(SIGINT, ft_print_n);
	term.fd_history = -1;
	term.line = NULL;
	term.history_cmd = NULL;
	read_file_history(&term);
	if (term.fd_history != -1)
		close(term.fd_history);
	term.fd_history = -1;
	while (1)
		teminal(&term);
	close(term.fd_history);
	return (0);
}
