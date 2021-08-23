/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 17:12:38 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/23 20:41:48 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cd(char ***command, int i, t_terminal *term)
{
	int	ret;

	free(term->status->line);
	term->status->line = ft_strdup("0");
	if (i >= 3)
	{
		ft_putstr_fd("cd: too many arguments\n", term->fd.error);
		return ;
	}
	if (i == 1)
		ret = chdir(getenv("HOME"));
	else
		ret = chdir(*(*command + 1));
	if (ret == -1)
	{
		print_error(*(*command + 1), strerror(errno), 4, term);
		return ;
	}
}

void	dupp(t_terminal *term)
{
	dup2(term->fd.in, 0);
	dup2(term->fd.out, 1);
	dup2(term->fd.error, 2);
}

void	pars_def_command(char ***command, t_terminal *term)
{
	t_pars_def_command	all;

	all.l = 0;
	update_variable_env(term, *command[0],
		ft_strrchr(*command[0], '/') + 1, NULL);
	term->flag.def_com = 1;
	all.pid = fork();
	if (all.pid == 0)
	{
		dupp(term);
		all.l = execve(*command[0], *command, term->start_env);
	}
	if (all.l == -1)
	{
		print_error(*command[0], strerror(errno), -1, term);
		exit(-1);
	}
}

void	pars_not_def_command(char ***command, t_terminal *term, int i)
{
	char	**tmp;
	int		j;
	int		size_arg;

	j = 0;
	size_arg = 0;
	tmp = *command;
	while (*(*command + size_arg) != NULL)
		size_arg++;
	while (tmp != NULL && *tmp != NULL)
	{
		tmp++;
		j++;
	}
	update_variable_env(term, NULL, *command[0], NULL);
	if (i == 0)
		ft_cd(command, j, term);
	else if (i == 1)
		ft_export(command, term, size_arg);
	else if (i == 2)
		ft_unset(command, term, size_arg);
	else if (i == 3)
		ft_env(term, 0, command);
}

void	pars_command(t_terminal *term, t_info_command **command_cur, int ret)
{
	term->fd = (*command_cur)->fd;
	if (ret && *((*command_cur)->command) != NULL
		&& ((*command_cur)->is_def_command
			|| (*command_cur)->number_command != -1))
	{
		if ((*command_cur)->number_command != -1)
			pars_not_def_command(&((*command_cur)->command),
				 term, (*command_cur)->number_command);
		else
			pars_def_command(&((*command_cur)->command), term);
	}
	else
	{
		free(term->status->line);
		term->status->line = ft_strdup("127");
	}
}
