/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 23:57:29 by dwanetta          #+#    #+#             */
/*   Updated: 2021/07/29 00:29:40 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char* envp[])
{
	struct termios term;
	char str[1000];

	tcgetattr(0, &term);
	term.c_cflag &= ~(ECHO);
	term.c_cflag &= ~(ICANON);
	tcsetattr(0, TCSANOW ,&term);
	read(1, str, 100);
	ft_putstr_fd(str, 1);
	return (0);
}
