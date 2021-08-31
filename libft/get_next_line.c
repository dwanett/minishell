/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 15:31:58 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/31 21:29:25 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_read_line(int fd, char *buf)
{
	int	size;
	int	check_n;

	check_n = 0;
	if (fd < 0)
		return (-1);
	size = read(fd, buf, BUFFER_SIZE);
	buf[size] = '\0';
	while (buf[check_n] != '\n' && buf[check_n] != '\0')
		check_n++;
	if (check_n < size)
		return (1);
	if (size == 0 || size < BUFFER_SIZE)
		return (0);
	if (size < 0)
		return (-1);
	return (1);
}

int	ft_save_buf(t_list_gnl **save, char *buf, int fd, int return_value)
{
	t_save_buf	all;
	t_list_gnl	*tmp;

	all.check_n = 0;
	tmp = *save;
	while (*save != NULL && (*save)->fd != fd)
		*save = (*save)->next;
	if (*save == NULL)
	{
		while (buf[all.check_n] != '\n')
			all.check_n++;
		all.size = ft_strlen(buf + all.check_n + 1);
		*save = (t_list_gnl *)malloc(sizeof(t_list_gnl));
		if (*save == NULL)
			return (-1);
		(*save)->str = (char *)malloc(sizeof(char) * all.size + 1);
		if ((*save)->str == NULL)
			return (-1);
		memcpy_gnl((*save)->str, buf + all.check_n + 1, all.size);
		(*save)->fd = fd;
		(*save)->return_vla = return_value;
		(*save)->next = tmp;
		return (1);
	}
	return (return_value);
}

int	ft_check(t_list_gnl **save, int fd)
{
	t_list_gnl	*tmp;

	tmp = *save;
	while (*save != NULL && (*save)->fd != fd)
		*save = (*save)->next;
	if (*save != NULL && (*save)->fd == fd)
	{
		*save = tmp;
		return (0);
	}
	*save = tmp;
	return (1);
}

int	get_next_line_help(t_start *al, char **line, int fd)
{
	while ((al->check_n == ft_strlen(al->buf)
			&& al->return_value > 0) || *line == NULL)
	{
		al->check_n = 0;
		al->return_value = ft_read_line(fd, al->buf);
		while (al->buf[al->check_n] != '\n' && al->buf[al->check_n] != '\0')
			al->check_n++;
		*line = ft_strjoin_new(*line, al->buf, al->check_n);
		if (*line == NULL)
			return (-1);
	}
	return (0);
}

int	get_next_line(int fd, char **line)
{
	t_start				al;
	static t_list_gnl	*save;

	al.check_n = 0;
	if (BUFFER_SIZE <= 0 || read(fd, al.buf, 0) < 0 || !line)
		return (-1);
	*line = NULL;
	if (!(*line) && save != NULL && (ft_check(&save, fd) == 0))
		al.return_value = ft_read_save(line, al.buf, &save, fd);
	else
	{
		if (get_next_line_help(&al, line, fd) == -1)
			return (-1);
		if (al.return_value == 1)
			al.return_value = ft_save_buf(&save, al.buf, fd, al.return_value);
	}
	return (al.return_value);
}
