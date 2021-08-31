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
	term->not_def_command[4] = "exit";
	read_file_history(term);
}

int	init_term_fd(t_terminal *term)
{
	if (term->fd.in != STDIN)
	{
		if (term->fd.in != -1)
			close(term->fd.in);
		term->fd.in = STDIN;
	}
	if (term->fd.out != STDOUT)
	{
		if (term->fd.out != -1)
			close(term->fd.out);
		term->fd.out = STDOUT;
	}
	if (term->fd.error != STDERROR)
	{
		if (term->fd.error != -1)
			close(term->fd.error);
		term->fd.error = STDERROR;
	}
	return (1);
}

int	check_valid_exit_val(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] == '-' || arg[i] == '+')
		i++;
	if (i != 1 && i != 0)
		return (1);
	while (arg[i] != '\0')
	{
		if (!ft_isdigit(arg[i]))
			return (1);
		i++;
	}
	return (0);
}
