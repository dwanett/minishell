/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 13:43:27 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/17 18:04:46 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

int	ft_strclen(const char *str, char c)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			break ;
		i++;
	}
	ret = i;
	return (ret);
}

int	ft_strcnlen(const char *str, char c, int n)
{
	int	i;
	int	count;
	int	ret;

	i = 0;
	count = 0;
	ret = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
		{
			count++;
			ret = i;
		}
		if (count == n)
			break ;
		i++;
	}
	return (ret);
}
