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
		return ;
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
		return ;
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

void free_history(t_terminal *term)
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
