/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 18:27:33 by dwanetta          #+#    #+#             */
/*   Updated: 2021/05/16 16:55:59 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

const char	*help_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	n;
	size_t	i;
	size_t	j;

	i = 0;
	while (haystack[i] != '\0' && i != len)
	{
		n = i;
		j = 0;
		if (haystack[i] == needle[j])
		{
			while (needle[j] != '\0' && n != len && *haystack != '\0')
			{
				if (haystack[n] != needle[j])
					break ;
				j++;
				n++;
				if (needle[j] == '\0')
					return (haystack + n - j);
			}
		}
		i++;
	}
	return (NULL);
}

const char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	if (!*needle)
		return (haystack);
	else
		return (help_strnstr(haystack, needle, len));
	return (NULL);
}
