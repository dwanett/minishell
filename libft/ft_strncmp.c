/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 11:57:44 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/12 21:29:39 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (((unsigned char)s1[i] != '\0'
			|| (unsigned char)s2[i] != '\0') && i < n)
	{
		if ((unsigned char)s1[i] > (unsigned char)s2[i])
			return (1);
		else if ((unsigned char)s1[i] < (unsigned char)s2[i])
			return (-1);
		i++;
	}
	return (0);
}

int	ft_strnccmp(const char *s1, const char *s2, char a, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	(void)n;
	while ((unsigned char)s1[j] == ' ')
		j++;
	while (((unsigned char)s1[j] != '\0'
			|| (unsigned char)s2[i] != '\0') && (unsigned char)s1[j] != a)
	{
		if ((unsigned char)s1[j] > (unsigned char)s2[i])
			return (1);
		else if ((unsigned char)s1[j] < (unsigned char)s2[i])
			return (-1);
		i++;
		j++;
	}
	return (0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (((unsigned char)s1[i] != '\0'
			|| (unsigned char)s2[i] != '\0'))
	{
		if ((unsigned char)s1[i] > (unsigned char)s2[i])
			return (1);
		else if ((unsigned char)s1[i] < (unsigned char)s2[i])
			return (-1);
		i++;
	}
	return (0);
}
