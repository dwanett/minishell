/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 18:23:06 by dwanetta          #+#    #+#             */
/*   Updated: 2021/05/16 16:51:33 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	char			*tmp;
	unsigned int	i;

	i = 0;
	if (s == NULL)
		return (NULL);
	tmp = (char *)s;
	while (i != n)
	{
		if (tmp[i] == c)
			return (tmp + i);
		i++;
	}
	return (NULL);
}
