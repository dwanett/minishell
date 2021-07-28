/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/20 09:56:50 by dwanetta          #+#    #+#             */
/*   Updated: 2021/05/16 16:53:47 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*buff;
	int		i;

	i = ft_strlen(src);
	buff = (char *)malloc(sizeof(char) * (i + 1));
	if (buff == NULL)
		return (NULL);
	else
	{
		i = 0;
		while (src[i++] != '\0')
			buff[i - 1] = src[i - 1];
		buff[i - 1] = '\0';
		return (buff);
	}
	return (NULL);
}
