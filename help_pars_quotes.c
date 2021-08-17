/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_pars_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 18:12:48 by gparsnip          #+#    #+#             */
/*   Updated: 2021/08/17 18:27:52 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	logic_quotes(char *command, int *i, char a, int *size)
{
	if (a == ' ')
	{
		while (command[*i] == a)
			(*i)++;
		(*size)++;
	}
	else
	{
		(*i)++;
		while (command[*i] != a)
		{
			if (command[*i] == '\0')
				return (-1);
			(*i)++;
		}
	}
	return (0);
}

int	ligic_quost_two(char *command)
{
	int	size;
	int	i;
	int	flag;

	i = 0;
	size = 1;
	flag = 0;
	while (command[i] != '\0')
	{
		if (command[i] == ' ')
			flag = logic_quotes(command, &i, ' ', &size);
		if (command[i] == '\'')
			flag = logic_quotes(command, &i, '\'', &size);
		if (command[i] == '"')
			flag = logic_quotes(command, &i, '"', &size);
		if (flag == -1)
			return (-1);
		i++;
	}
	return (size);
}

void	count_bad(char *command, int *i)
{
	int	size;

	size = 0;
	if (command[*i] == ' ')
	{
		while (command[*i] == ' ')
			*i = *i + 1;
	}
	while (command[*i] != '\0' && command[*i] != ' ')
	{
		if (command[*i] == '\'')
			logic_quotes(command, i, '\'', &size);
		if (command[*i] == '"')
			logic_quotes(command, i, '"', &size);
		(*i)++;
	}
}

void	command_pipe_null(char ****command_pipe, int i, int *ret)
{
	(*command_pipe)[i] = NULL;
	*ret = 0;
}
