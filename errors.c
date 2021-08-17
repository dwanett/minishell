/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 16:02:56 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/17 18:59:50 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(char *msg, char *error, int flag, t_terminal *term)
{
	if (flag == 0)
	{
		ft_putstr_fd("FATAL ERROR\n", 2);
		ft_putstr_fd(error, 2);
		ft_putstr_fd("\n", 2);
		exit(errno);
	}
	if (flag == 1)
		ft_putstr_fd("env: ", term->fd.error);
	if (flag == 2)
		ft_putstr_fd("unset: ", term->fd.error);
	if (flag == 3)
		ft_putstr_fd("export: ", term->fd.error);
	if (flag == 4)
		ft_putstr_fd("cd: ", term->fd.error);
	ft_putstr_fd(msg, term->fd.error);
	ft_putstr_fd(": ", term->fd.error);
	ft_putstr_fd(error, term->fd.error);
	ft_putstr_fd("\n", term->fd.error);
	free(term->status->line);
	if (flag == 1 || flag == 2 || flag == 3 || flag == 4)
		term->status->line = ft_strdup("1");
	else
		term->status->line = ft_strdup("127");
}
