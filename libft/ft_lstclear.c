/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 20:11:40 by dwanetta          #+#    #+#             */
/*   Updated: 2020/11/04 22:17:48 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	void	*tmp;

	if (*lst && lst)
	{
		while (*lst)
		{
			tmp = (*lst)->next;
			(*del)(*lst);
			*lst = tmp;
		}
		(*lst) = NULL;
	}
}
