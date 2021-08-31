/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_info_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:57:41 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/17 17:03:34 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tmp_variable(char ***command, t_terminal *term, int i)
{
	t_list_env	*tmp;

	while ((*command)[i] != NULL)
	{
		if (count_symbol_str((*command)[i], '=') == 0)
			return (1);
		i++;
	}
	i = -1;
	while ((*command)[++i] != NULL)
	{
		tmp = (t_list_env *)malloc(sizeof(t_list_env));
		if (tmp == NULL)
			print_error(NULL, strerror(errno), 0, term);
		tmp->update_variable = NULL;
		tmp->name = ft_strndup((*command)[i], ft_strclen((*command)[i], '='));
		tmp->line = ft_strdup((*command)[i]
				+ ft_strclen((*command)[i], '=') + 1);
		tmp->tmp_variable = 1;
		tmp->next = term->env;
		term->env = tmp;
		term->flag.export = 2;
	}
	update_variable_env(term, NULL, "", NULL);
	return (0);
}

void	init_list_command(t_info_command **tmp,
		t_terminal *term, char **command_pipe, int ret)
{
	(*tmp)->is_def_command = 0;
	(*tmp)->command = command_pipe;
	if (ret && *((*tmp)->command) != NULL)
	{
		(*tmp)->number_command = check_not_def_com(*((*tmp)->command),
				term->not_def_command);
		if ((*tmp)->number_command == -1
			&& tmp_variable(&((*tmp)->command), term, 0))
			(*tmp)->is_def_command = check_def_command(&((*tmp)->command),
					term);
	}
}

void	get_fd_str_command(t_info_command **tmp,
		int i, int *fd, char ***command_pipe)
{
	if (i != 0)
		(*tmp)->fd.in = fd[0];
	pipe(fd);
	if ((*tmp)->fd.out == STDOUT)
	{
		if (command_pipe[i + 1] == NULL)
			(*tmp)->fd.out = STDOUT;
		else
			(*tmp)->fd.out = fd[1];
	}
	else
	{
		write(fd[1], "\0", 1);
		close(fd[1]);
	}
}

void	get_info_str_command(t_info_command **command_cur,
	t_terminal *term, char ***command_pipe, int ret)
{
	t_info_command	*tmp;
	int				fd[2];
	int				i;

	i = 0;
	fd[0] = STDIN;
	fd[1] = -1;
	tmp = (*command_cur);
	while (command_pipe[i] != NULL)
	{
		get_fd_str_command(&tmp, i, fd, command_pipe);
		init_list_command(&tmp, term, command_pipe[i], ret);
		tmp = tmp->next;
		i++;
	}
}

void	ft_status(t_terminal *term, int status, int *er)
{
	waitpid(term->pip, &status, 0);
	if (status == 2)
		ft_putstr_fd("\n", 1);
	signal(SIGINT, ft_print_n);
	if (*er == 0)
	{
		free(term->status->line);
		if (status == 2)
			term->status->line = ft_strdup("130");
		else
			term->status->line = ft_strdup(ft_itoa(WEXITSTATUS(status)));
	}
	if (term->pip != 0)
	{
		kill(term->pip, SIGKILL);
	}
}
