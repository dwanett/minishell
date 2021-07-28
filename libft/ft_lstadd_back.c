/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 19:57:53 by dwanetta          #+#    #+#             */
/*   Updated: 2021/05/16 16:47:45 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	void	*tmp;

	if (lst)
	{
		if (!*lst && new)
			*lst = new;
		else if (new)
		{
			tmp = (void *)*lst;
			*lst = ft_lstlast(*lst);
			(*lst)->next = new;
			*lst = tmp;
		}
	}
}
