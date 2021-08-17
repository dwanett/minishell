/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 18:09:36 by gparsnip          #+#    #+#             */
/*   Updated: 2021/08/17 18:39:47 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	count_char_utils(char *command, int *quotes, t_char_record *all)
{
	if (command[(*all).j] == '\'')
	{
		(*all).g++;
		while (command[(*all).g] != '\'')
			(*all).g++;
		*quotes += 2;
	}
	if (command[(*all).j] == '"')
	{
		(*all).g++;
		while (command[(*all).g] != '"')
			(*all).g++;
		*quotes += 2;
	}
	(*all).g++;
}

int	count_char(char *command, int count)
{
	t_char_record	all;
	int				quotes;

	all.i = 0;
	all.g = 0;
	quotes = 0;
	all.j = 0;
	while (count != 0)
	{
		count_bad(command, &(all.i));
		count--;
	}
	while (command[all.i] == ' ')
		all.i++;
	all.j = all.i;
	count_bad(command, &(all.j));
	all.g = all.i;
	while (all.g != all.j)
		count_char_utils(command, &quotes, &all);
	all.j = all.j - quotes;
	return (all.j - all.i);
}

void	char_record_utils(char **a,
	char *command, t_char_record *all, char symb)
{
	if (symb != '\0')
	{
		(*all).i++;
		while (command[(*all).i] != symb)
		{
			(*a)[(*all).g] = command[(*all).i];
			(*all).i++;
			(*all).g++;
		}
		(*all).i++;
	}
	else
	{
		(*a)[(*all).g] = command[(*all).i];
		(*all).i++;
		(*all).g++;
	}
}

void	char_record(char **a, int count, char *command)
{
	t_char_record	all;

	all.i = 0;
	all.j = 0;
	all.g = 0;
	while (count != 0)
	{
		count_bad(command, &(all.i));
		count--;
	}
	while (command[all.i] == ' ')
		all.i++;
	all.j = all.i;
	count_bad(command, &(all.j));
	while (all.i != all.j)
	{
		if (command[all.i] == '\'')
			char_record_utils(a, command, &all, '\'');
		else if (command[all.i] == '"')
			char_record_utils(a, command, &all, '"');
		else
			char_record_utils(a, command, &all, '\0');
	}
	(*a)[all.g] = '\0';
}

int	pars_quotes(char **command, t_terminal *term, char ****command_pipe, int i)
{
	int	size;
	int	count;

	size = ligic_quost_two(*command);
	count = 0;
	if (size == -1)
	{
		print_error(*command, "error syntax", -1, term);
		return (1);
	}
	(*command_pipe)[i] = (char **)malloc(sizeof(char **) * (size + 1));
	if ((*command_pipe)[i] == NULL)
		print_error(NULL, strerror(errno), 0, term);
	(*command_pipe)[i][size] = NULL;
	while (size != 0)
	{
		(*command_pipe)[i][count] = (char *)malloc
			(count_char(*command, count) + 1);
		if ((*command_pipe)[i][count] == NULL)
			print_error(NULL, strerror(errno), 0, term);
		char_record(&((*command_pipe)[i][count]), count, *command);
		size--;
		count++;
	}
	return (0);
}
