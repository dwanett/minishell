/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:35:21 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/17 16:42:23 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_t_teminal(t_terminal *term, int argc, char **argv, char **envp)
{
	term->update = NULL;
	init_env(&term->env, envp, term, 0);
	init_env_for_next_process(term, term->env);
	(void)argc;
	(void)argv;
	term->fd.history = -1;
	term->flag.export = 0;
	term->flag.def_com = 0;
	term->flag.error = 0;
	term->line = NULL;
	term->history_cmd = NULL;
	term->fd.in = STDIN;
	term->fd.out = STDOUT;
	term->fd.error = STDERROR;
	term->not_def_command[0] = "cd";
	term->not_def_command[1] = "export";
	term->not_def_command[2] = "unset";
	term->not_def_command[3] = "env";
	read_file_history(term);
}

int	init_term_fd(t_terminal *term)
{
	if (term->fd.in != STDIN)
	{
		close(term->fd.in);
		term->fd.in = STDIN;
	}
	if (term->fd.out != STDOUT)
	{
		close(term->fd.out);
		term->fd.out = STDOUT;
	}
	if (term->fd.error != STDERROR)
	{
		close(term->fd.error);
		term->fd.error = STDERROR;
	}
	return (1);
}
