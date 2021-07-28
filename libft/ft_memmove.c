/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 16:05:38 by dwanetta          #+#    #+#             */
/*   Updated: 2020/11/03 21:01:02 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char			*tmp;
	const char		*tmp2;
	unsigned int	i;

	i = 0;
	if (dst != NULL || src != NULL)
	{
		tmp = dst;
		tmp2 = src;
		if (dst < src)
		{
			while (i++ != len)
				tmp[i - 1] = tmp2[i - 1];
		}
		else
		{
			i = len;
			while (i != 0)
			{
				tmp[i - 1] = tmp2[i - 1];
				i--;
			}
		}
	}
	return (dst);
}
