/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 14:00:47 by gparsnip          #+#    #+#             */
/*   Updated: 2021/08/19 15:33:45 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_split_help(char *line, int *i)
{
	if (line[*i] == '\'')
	{
		*i = *i + 1;
		while (line[*i] != '\'' && line[*i] != '\0')
			*i = *i + 1;
	}
	if (line[*i] == '"')
	{
		*i = *i + 1;
		while (line[*i] != '"' && line[*i] != '\0')
			*i = *i + 1;
	}
}

int	ft_count(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 1;
	while (line[i] != '\0')
	{
		ft_split_help(line, &i);
		if (line[i] == '|')
			count++;
		if (line[i] != '\0')
			i++;
	}
	return (count);
}

char	*ft_line(int *i, char *line)
{
	char	*count;
	int		j;
	int		k;

	j = *i;
	k = 0;
	while (line[j] != '\0' && line[j] != '|')
	{
		ft_split_help(line, &j);
		if (line[j] != '\0')
			j++;
	}
	count = malloc(j - *i + 2);
	while (*i != j)
	{
		count[k] = line[*i];
		k++;
		*i = *i + 1;
	}
	if (line[*i] == '|')
		*i = *i + 1;
	count[k] = '\0';
	return (count);
}

char	**ft_split_pipe(char *line)
{
	char	**split;
	int		count;
	int		i;
	int		j;

	count = ft_count(line);
	i = 0;
	j = 0;
	split = (char **)malloc(sizeof(char *) * (count + 1));
	while (count != 0)
	{
		split[j] = ft_line(&i, line);
		j++;
		count--;
	}
	split[j] = NULL;
	return (split);
}
