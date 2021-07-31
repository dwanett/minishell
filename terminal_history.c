/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 15:32:17 by dwanetta          #+#    #+#             */
/*   Updated: 2021/07/31 15:32:17 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_add_history(t_terminal *term) // сохраниение истории текущий сессии
{
	t_list_histoy *tmp;

	tmp = (t_list_histoy *)malloc(sizeof(t_list_histoy));
	tmp->command = term->line;
	if (term->history_cmd == NULL)
	{
		tmp->prev = term->history_cmd;
		tmp->next = NULL;
	}
	else
	{
		tmp->next = term->history_cmd;
		term->history_cmd->prev = tmp;
		tmp->prev = NULL;
	}
	term->history_cmd = tmp;
}

void read_file_history(t_terminal *term) //чтение истории предыдущих сессий
{
	int l;
	char buf[1];
	char *str;

	l = 1;
	term->fd_history = open(".history", O_RDWR);
	if (term->fd_history == -1 && errno == 2)
		return;
	else if (term->fd_history == -1)
	{
		ft_putstr_fd(strerror(errno), 2);
		exit(errno);
	}
	while (l != 0)
	{
		str = (char *)malloc(sizeof(char));
		while (1)
		{
			l = read(term->fd_history, buf, 1);
			if (l == 0 || *buf == '\n')
				break;
			if (term->line == NULL)
			{
				term->line = ft_strjoin(str, buf);
				free(str);
			}
			else
			{
				str = ft_strjoin(term->line, buf);
				free(term->line);
				term->line = str;
			}
		}
		if (l != 0)
		{
			add_history(term->line);
			ft_add_history(term);
		}
		else
			free(str);
		term->line = NULL;
		str = NULL;
	}
}

void save_history(t_terminal *term) // сохранение истории в файл
{
	t_list_histoy *tmp;

	tmp = term->history_cmd;
	if (tmp == NULL)
		return;
	if (term->fd_history == -1)
		term->fd_history = open(".history", O_CREAT | O_RDWR, S_IRWXU);
	if (term->fd_history == -1)
		term->fd_history = open(".history", O_TRUNC | O_RDWR);
	if (term->fd_history == -1)
	{
		ft_putstr_fd(strerror(errno), 2);
		exit(errno);
	}
	else
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		while (tmp != NULL)
		{
			ft_putstr_fd(tmp->command, term->fd_history);
			ft_putstr_fd("\n", term->fd_history);
			tmp = tmp->prev;
		}
	}
}