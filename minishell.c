/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 23:57:29 by dwanetta          #+#    #+#             */
/*   Updated: 2021/07/29 21:13:58 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int serch_term_name(char **envp) // Поиск названия терминала в пространстве имен
{
	int i;

	i = 0;
	while (ft_strncmp("TERM=", envp[i], 5))
	{
	    i++;
	    if (envp[i] == NULL)
            return (0);
	}
	return (i);
}

int	main(int argc, char **argv, char **envp)
{
	struct termios term;
	char str[1000];
	int i;
	int l;
	int	count;
	int	len_str;

	count = 0;
	len_str = 0;
	tcgetattr(0, &term);
	term.c_lflag &= ~(ECHO);
	term.c_lflag &= ~(ICANON);
	tcsetattr(0, TCSANOW ,&term);
	i = serch_term_name(envp);
	tgetent(0, envp[i] + 5);
	while (ft_strcmp(str, "\4"))
	{
		ft_putstr_fd("minishell$ ", 1);
		tputs(save_cursor, 1, ft_putchar);
		count = 0;
		while (1)
		{
			//ft_putnbr_fd(count, 1);
			//ft_putstr_fd("minishell$ ", 1);
			l = read(0, str, 100);
			if (!ft_strncmp(str, "\e[A", 3))
			{
				tputs(restore_cursor, 1, ft_putchar);
				tputs(tigetstr("ed"), 1, ft_putchar);
				ft_putstr_fd("up", 1);
				count +=2;
				len_str +=2;
			}
			else if(!ft_strncmp(str, "\e[B", 3))
			{
				tputs(restore_cursor, 1, ft_putchar);
				tputs(tigetstr("ed"), 1, ft_putchar);
				ft_putstr_fd("down", 1);
				count +=4;
				len_str +=4;
			}
			else if (!ft_strncmp(str, "\e[D", 3))
			{
				if (count > 0)
				{
					tputs(cursor_left, 1, ft_putchar);
					count--;
				}
			}
			else if (!ft_strncmp(str, "\e[C", 3))
			{
				if (count < len_str)
				{
					tputs(cursor_right, 1, ft_putchar);
					count++;
				}
			}
			else if(!ft_strcmp(str, "\177") && count > 0)
			{
				tputs(cursor_left, 1, ft_putchar);
				tputs(delete_character, 1, ft_putchar);
				count--;
				len_str--;
			}
			else
			{
				if (ft_strcmp(str, "\177"))
				{
					count++;
					len_str++;
				}
				write(1, str, l);
			}
			//ft_putnbr_fd(count, 1);
			if (!ft_strcmp(str, "\n") || !ft_strcmp(str, "\4"))
				break ;
			ft_bzero(str, l);
			//STRELOCHKI
		}
	}
	write(1, "\n", 1);
	return (0);
}
