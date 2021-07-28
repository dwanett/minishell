/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 21:01:55 by dwanetta          #+#    #+#             */
/*   Updated: 2020/11/06 17:27:07 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	help_lst_map(t_list *lst, t_list **new_elem, t_list	**new_lst,
			void *(*f)(void *), void (*del)(void *))
{
	while (lst)
	{
		*new_elem = ft_lstnew(f(lst->content));
		if (!*new_elem)
		{
			ft_lstclear(&lst, del);
			ft_lstclear(new_lst, del);
			break ;
		}
		lst = lst->next;
		ft_lstadd_back(new_lst, *new_elem);
	}
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *),
				void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*new_elem;

	if (!lst || !f)
		return (NULL);
	new_elem = ft_lstnew(f(lst->content));
	if (!new_elem)
	{
		ft_lstclear(&lst, del);
		return (NULL);
	}
	new_lst = new_elem;
	lst = lst->next;
	help_lst_map(lst, &new_elem, &new_lst, f, del);
	return (new_lst);
}
