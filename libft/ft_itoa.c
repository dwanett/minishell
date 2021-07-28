/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 14:52:44 by dwanetta          #+#    #+#             */
/*   Updated: 2021/05/16 16:46:51 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_malc(int n, int i)
{
	char	*result;

	result = (char *)malloc(sizeof(char) * (i + 1));
	if (result == NULL)
		return (NULL);
	if (n < 0)
		*result = '-';
	result[i] = '\0';
	return (result);
}

int	ft_chek(int tmp)
{
	int		i;

	i = 0;
	while (1)
	{
		tmp /= 10;
		i++;
		if (tmp == 0)
			break ;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	int		tmp;
	int		i;
	int		minus;
	char	*result;

	tmp = n;
	minus = 1;
	i = ft_chek(tmp);
	if (n < 0)
	{
		minus = -1;
		i++;
	}
	result = ft_malc(n, i);
	if (result == NULL)
		return (NULL);
	i -= 1;
	while (i >= 0 && result[i] != '-')
	{
		result[i--] = 48 + ((n % 10) * minus);
		n /= 10;
	}
	return (result);
}
