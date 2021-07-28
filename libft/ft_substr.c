/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 13:02:58 by dwanetta          #+#    #+#             */
/*   Updated: 2021/05/16 16:57:27 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*str;
	int				i;
	unsigned int	chek_size;

	i = 0;
	if (s == NULL)
		return (NULL);
	chek_size = ft_strlen(s);
	if (start >= chek_size)
		len = 0;
	else if (chek_size <= len)
		len = chek_size;
	else if (len + start > chek_size)
		len = chek_size - start;
	str = (char *)malloc(sizeof(char) * len + 1);
	if (str == NULL)
		return (NULL);
	len += start;
	while (start < len)
		str[i++] = s[start++];
	str[i] = '\0';
	return (str);
}
