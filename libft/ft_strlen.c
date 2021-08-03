/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 13:43:27 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/03 14:35:59 by dwanetta         ###   ########.fr       */
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
	int ret;

	i = 0;
	ret = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
		{
			ret = i;
			break ;
		}
		i++;
	}
	return (ret);
}

int ft_strcnlen(const char *str, char c, int n)
{
	int i;
	int count;
	int ret;

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