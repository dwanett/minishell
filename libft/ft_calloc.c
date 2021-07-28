/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 18:21:45 by dwanetta          #+#    #+#             */
/*   Updated: 2021/05/16 16:03:26 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*arr;
	int		full;

	full = nmemb * size;
	arr = (void *)malloc(full);
	if (arr == NULL)
		return (NULL);
	ft_bzero(arr, full);
	return (arr);
}
