/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_history.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 15:32:17 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/16 16:47:38 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// сохраниение истории текущий сессии
void	ft_add_history(t_terminal *term)
{
	t_list_histoy	*tmp;

	tmp = (t_list_histoy *)malloc(sizeof(t_list_histoy));
	if (tmp == NULL)
		print_error(NULL, strerror(errno), 0, term);
	tmp->command = ft_strdup(term->line);
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

void	read_file_history_help(t_terminal *term, int *l, char **str)
{
	char	buf[2];

	(*str) = (char *)malloc(sizeof(char));
	if ((*str) == NULL)
		print_error(NULL, strerror(errno), 0, term);
	(*str)[0] = '\0';
	while (1)
	{
		*l = read(term->fd.history, &buf[0], 1);
		buf[1] = '\0';
		if (*l == 0 || *buf == '\n')
			break ;
		if (term->line == NULL)
		{
			term->line = ft_strjoin((*str), buf);
			free((*str));
		}
		else
		{
			(*str) = ft_strjoin(term->line, buf);
			free(term->line);
			term->line = (*str);
		}
	}
}

//чтение истории предыдущих сессий
void	read_file_history(t_terminal *term)
{
	int		l;
	char	*str;

	l = 1;
	term->fd.history = open(".history", O_RDWR);
	if (term->fd.history == -1 && errno == 2)
		return ;
	else if (term->fd.history == -1)
		print_error(NULL, strerror(errno), 0, term);
	while (l != 0)
	{
		read_file_history_help(term, &l, &str);
		if (l != 0 && term->line != NULL)
		{
			add_history(term->line);
			ft_add_history(term);
		}
		free(str);
		term->line = NULL;
		str = NULL;
	}
}

// сохранение истории в файл
void	save_history(t_terminal *term)
{
	t_list_histoy	*tmp;

	tmp = term->history_cmd;
	if (tmp == NULL)
		return ;
	if (term->fd.history == -1)
		term->fd.history = open(".history", O_CREAT | O_RDWR, S_IRWXU);
	if (term->fd.history == -1)
		term->fd.history = open(".history", O_TRUNC | O_RDWR);
	if (term->fd.history == -1)
		print_error(NULL, strerror(errno), 0, term);
	else
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		while (tmp != NULL)
		{
			ft_putstr_fd(tmp->command, term->fd.history);
			ft_putstr_fd("\n", term->fd.history);
			tmp = tmp->prev;
		}
	}
}
