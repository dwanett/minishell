/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 17:34:27 by dwanetta          #+#    #+#             */
/*   Updated: 2021/05/16 16:57:20 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_startcount(char const *s1, char const *set)
{
	int	f;
	int	index;
	int	length;
	int	len;
	int	i;

	f = 1;
	length = ft_strlen(s1);
	len = ft_strlen(set);
	index = 0;
	f = 1;
	while (index < length - 1 && f == 1)
	{
		f = 0;
		i = 0;
		while (i++ < len)
			if (s1[index] == set[i - 1])
				f = 1;
		if (f == 0)
			break ;
		index++;
	}
	return (index);
}

int	ft_endcount(char const *s1, char const *set)
{
	int	f;
	int	index;
	int	length;
	int	len;
	int	i;

	f = 1;
	length = ft_strlen(s1);
	len = ft_strlen(set);
	index = 0;
	f = 1;
	while (length > index && f == 1)
	{
		f = 0;
		i = 0;
		while (i++ < len)
			if (s1[length - 1] == set[i - 1])
				f = 1;
		if (f == 0)
			break ;
		length--;
	}
	return (length);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*output;
	int		a;
	int		length;
	int		index;

	if (s1 == NULL || set == NULL)
		return (NULL);
	index = ft_startcount(s1, set);
	length = ft_endcount(s1, set);
	if ((length - index) > 0)
		output = (char *)malloc(sizeof(char) * (length - index + 1));
	else
	{
		output = (char *)malloc(sizeof(char));
		output[0] = '\0';
		return (output);
	}
	if (output == NULL)
		return (NULL);
	a = 0;
	while (index++ < length)
		output[a++] = s1[index - 1];
	output[a] = '\0';
	return (output);
}
