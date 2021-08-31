/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 15:32:01 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/31 21:28:52 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strjoin_new(char *s1, char *s2, int check_n)
{
	t_strjoin_new	all;

	all.i = 0;
	all.j = 0;
	all.k = 0;
	all.size = ft_strlen(s2);
	if (check_n != all.size && all.size != 0)
		all.size = check_n;
	all.str = (char *)malloc(sizeof(char) * (all.size + ft_strlen(s1) + 1));
	if (all.str == NULL)
		return (NULL);
	while ((s2[all.j] != '\0' && s2[all.j] != '\n')
		|| (s1 != NULL && s1[all.k] != '\0'))
	{
		if (s1 != NULL && s1[all.k] != '\0')
			all.str[all.i] = s1[all.k++];
		else
			all.str[all.i] = s2[all.j++];
		all.i++;
	}
	all.str[all.i] = '\0';
	free(s1);
	return (all.str);
}

void	memcpy_gnl(void *dst, void *src, int n)
{
	char	*tmp;
	char	*tmp2;
	int		i;

	i = 0;
	tmp = (char *)dst;
	tmp2 = (char *)src;
	if (n < 0)
		n = 0;
	if (tmp != NULL || tmp2 != NULL)
	{
		while (i != n)
		{
			tmp[i] = tmp2[i];
			i++;
		}
	}
	tmp[i] = '\0';
}

int	ft_add_in_save(char *buf, t_list_gnl **save, int fd, int check_n)
{
	int		return_value;
	int		size_save_str;
	char	*tmp;

	size_save_str = ft_strlen((*save)->str);
	return_value = 1;
	while (check_n == size_save_str && return_value != 0)
	{
		check_n = 0;
		return_value = ft_read_line(fd, buf);
		tmp = (*save)->str;
		(*save)->str = (char *)malloc(sizeof(char)
				* (size_save_str + ft_strlen(buf) + 1));
		if ((*save)->str == NULL)
			return (-1);
		memcpy_gnl((*save)->str, tmp, size_save_str);
		memcpy_gnl((*save)->str + size_save_str, buf, ft_strlen(buf));
		free(tmp);
		while ((*save)->str[check_n] != '\n' && (*save)->str[check_n] != '\0')
			check_n++;
		size_save_str = ft_strlen((*save)->str);
	}
	(*save)->return_vla = return_value;
	return (check_n);
}

void	ft_free_save(t_list_gnl **root, t_list_gnl **del)
{
	t_list_gnl	*tmp_save;

	tmp_save = *root;
	while (tmp_save != NULL && tmp_save->next != *del && tmp_save != *del)
		tmp_save = tmp_save->next;
	if (tmp_save == *del)
		*root = (*root)->next;
	else if ((*del)->next == NULL)
		tmp_save->next = NULL;
	else
		tmp_save->next = (*del)->next;
	free((*del)->str);
	(*del)->next = NULL;
	free(*del);
}

int	ft_read_save(char **line, char *buf, t_list_gnl **save, int fd)
{
	t_read_save	al;

	al.n = 0;
	al.tmp_save = *save;
	while (*save != NULL && (*save)->fd != fd)
		*save = (*save)->next;
	if (*save != NULL)
	{
		while ((*save)->str[al.n] != '\n' && (*save)->str[al.n] != '\0')
			al.n++;
		if (al.n == ft_strlen((*save)->str))
			al.n = ft_add_in_save(buf, save, fd, al.n);
		*line = ft_strjoin_new(*line, (*save)->str, al.n);
		al.tmp = (*save)->str;
		(*save)->str = malloc(sizeof(char) * ft_strlen(al.n + al.tmp) + 1);
		if ((*save)->str == NULL)
			return (-1);
		memcpy_gnl((*save)->str, al.tmp + al.n + 1, ft_strlen(al.n + al.tmp));
		free(al.tmp);
	}
	fd = (*save)->return_vla;
	if (fd == 0)
		ft_free_save(&(al.tmp_save), save);
	*save = al.tmp_save;
	return (fd);
}
