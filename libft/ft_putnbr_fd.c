/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 15:34:06 by dwanetta          #+#    #+#             */
/*   Updated: 2021/05/16 16:52:46 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_chek_nbr(int tmp)
{
	int	i;

	i = 1;
	while (1)
	{
		tmp /= 10;
		if (tmp == 0)
			break ;
		i *= 10;
	}
	return (i);
}

void	ft_putnbr_fd(int n, int fd)
{
	int	i;
	int	minus;

	minus = 1;
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		minus = -1;
	}
	i = ft_chek_nbr(n);
	if (n == 0)
		ft_putchar_fd('0', fd);
	else
	{
		while (i != 0)
		{
			ft_putchar_fd((char)(48 + ((n / i) * minus)), fd);
			n %= i;
			i /= 10;
		}
	}
}
