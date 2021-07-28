/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 13:54:33 by dwanetta          #+#    #+#             */
/*   Updated: 2021/05/16 16:52:11 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char			*tmp;
	char			*tmp2;
	unsigned int	i;

	i = 0;
	tmp = (char *)dst;
	tmp2 = (char *)src;
	if (tmp != NULL || tmp2 != NULL)
	{
		while (i != n)
		{
			tmp[i] = tmp2[i];
			i++;
		}
	}
	return (dst);
}
