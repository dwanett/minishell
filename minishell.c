/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 23:57:29 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/23 21:11:38 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_symbol_str(const char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count);
}

void	last_arg(t_terminal *term, t_info_command **command_cur, int *j)
{
	while ((*command_cur)->command != NULL
		&& (*command_cur)->command[*j] != NULL)
	{
		if ((*command_cur)->command[*j + 1] == NULL
			&& term->flag.export != 2 && term->flag.def_com == 0)
			update_variable_env(term, NULL, (*command_cur)->command[*j], NULL);
		if (*j == 0)
			term->flag.def_com = 0;
		free((*command_cur)->command[*j]);
		(*j)++;
	}
}

void	command(t_terminal *term, int status, int j, int er)
{
	char			***command_pipe;
	t_info_command	*command_cur;
	t_info_command	*tmp;
	int				ret;

	ret = pre_pars(term, &command_pipe, &command_cur, -1);
	get_info_str_command(&command_cur, term, command_pipe, ret);
	term->pip = 0;
	while (command_cur != NULL)
	{
		j = 0;
		er = 0;
		pars_command(term, &command_cur, ret, &er);
		last_arg(term, &command_cur, &j);
		if (term->fd.out != STDOUT && term->fd.out != -1)
			close(term->fd.out);
		if (term->fd.in != STDIN && term->fd.in != -1)
			close(term->fd.in);
		free(command_cur->command);
		tmp = command_cur;
		command_cur = command_cur->next;
		free(tmp);
	}
	ft_status(term, status, &er);
	free(command_pipe);
}

//???????????? ?????????? ??????????????????
void	teminal(t_terminal *term)
{
	if (init_term_fd(term) && term->line != NULL)
		free(term->line);
	term->line = readline("minishell$ ");
	if (term->line == NULL)
		ft_exit(term, NULL);
	else if (ft_strcmp(term->line, "\0"))
	{
		if (term->history_cmd)
		{
			if (ft_strcmp(term->line, term->history_cmd->command))
			{
				ft_add_history(term);
				add_history(term->line);
			}
		}
		else
		{
			ft_add_history(term);
			add_history(term->line);
		}
		command(term, 0, 0, 0);
		if (term->flag.error == 1)
			ft_putstr_fd(";: error syntax\n", term->fd.error);
		term->flag.error = 0;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_terminal	term;

	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, ft_print_n);
	init_t_teminal(&term, argc, argv, envp);
	if (term.fd.history != -1)
		close(term.fd.history);
	term.fd.history = -1;
	while (1)
		teminal(&term);
	close(term.fd.history);
	return (0);
}
