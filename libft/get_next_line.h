/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 15:31:56 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/31 21:29:32 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# define BUFFER_SIZE  1000000

# include "libft.h"
# include <unistd.h>
# include <stdlib.h>

typedef struct s_list_gnl
{
	char				*str;
	int					return_vla;
	int					fd;
	struct s_list_gnl	*next;
}					t_list_gnl;

typedef struct s_start
{
	char			buf[BUFFER_SIZE + 1];
	int				return_value;
	int				check_n;
}					t_start;

typedef struct s_save_buf
{
	int		check_n;
	int		size;
}					t_save_buf;

typedef struct s_strjoin_new
{
	char	*str;
	int		i;
	int		j;
	int		k;
	int		size;
}					t_strjoin_new;

typedef struct s_read_save
{
	t_list_gnl	*tmp_save;
	int			n;
	char		*tmp;
}					t_read_save;

int					ft_read_save(char **line, char *buf,
						t_list_gnl **save, int fd);
int					ft_read_line(int fd, char *buf);
void				memcpy_gnl(void *dst, void *src, int n);
char				*ft_strjoin_new(char *s1, char *s2, int check_n);
int					get_next_line(int fd, char **line);

#endif
