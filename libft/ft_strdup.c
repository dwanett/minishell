/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/20 09:56:50 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/17 18:03:53 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

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

char	*ft_strndup(const char *src, int n)
{
	char	*buff;
	int		i;

	i = n;
	buff = (char *)malloc(sizeof(char) * (i + 1));
	if (buff == NULL)
		return (NULL);
	else
	{
		i = 0;
		while (src[i++] != '\0')
		{
			buff[i - 1] = src[i - 1];
			if (i - 1 == n)
				break ;
		}
		buff[i - 1] = '\0';
		return (buff);
	}
	return (NULL);
}

void	ft_strdub_quotes_help(const char *src, char **buff)
{
	int	i;
	int	j;

	(*buff)[0] = '\'';
	i = 0;
	j = 1;
	while (src[i] != '\0')
	{
		if (src[i] == '\'')
		{
			(*buff)[j] = '\'';
			(*buff)[j + 1] = '"';
			(*buff)[j + 2] = '\'';
			(*buff)[j + 3] = '"';
			(*buff)[j + 4] = '\'';
			j += 4;
		}
		else
			(*buff)[j] = src[i];
		j++;
		i++;
	}
	(*buff)[j] = '\'';
	(*buff)[j + 1] = '\0';
}

char	*ft_strdup_quotes(const char *src)
{
	char	*buff;
	int		i;
	int		j;

	j = 0;
	i = 0;
	while (src[j] != '\0')
	{
		if (src[j] == '\'')
			i += 4;
		j++;
	}
	j = i;
	i = ft_strlen(src);
	buff = (char *)malloc(sizeof(char) * (i + 3 + j));
	if (buff == NULL)
		return (NULL);
	else
		ft_strdub_quotes_help(src, &buff);
	return (buff);
}
