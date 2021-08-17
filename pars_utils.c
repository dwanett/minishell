/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 18:16:05 by gparsnip          #+#    #+#             */
/*   Updated: 2021/08/17 18:39:55 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_size_matrix_and_trim(char **matrix,
	char *c, t_terminal *term, char ****command_pipe)
{
	int		i;
	char	*tmp;

	i = 0;
	while (matrix[i] != NULL)
	{
		if (c)
		{
			tmp = matrix[i];
			matrix[i] = ft_strtrim(tmp, c);
			free(tmp);
		}
		i++;
	}
	*command_pipe = (char ***)malloc(sizeof(char **) * (i + 1));
	if (*command_pipe == NULL)
		print_error(NULL, strerror(errno), 0, term);
	return (i);
}

char	*serch_env(char *name, t_terminal *term, int *i)
{
	t_list_env	*tmp;
	int			j;

	tmp = term->env;
	j = 0;
	while (tmp != NULL)
	{
		*i = ft_strlen(tmp->name);
		if (!ft_strncmp(name, tmp->name, *i))
		{
			if (!ft_strcmp(tmp->name, "_"))
			{
				if (term->update == NULL)
					update_variable_env(term, NULL, "_", NULL);
				return (term->update->update_variable);
			}
			return (tmp->line);
		}
		tmp = tmp->next;
	}
	while (ft_isalpha(name[j]))
		j++;
	*i = j;
	return ("");
}

void	pars_env_elem_utils(char **command_cur,
	t_terminal *term, t_pars_env_elem *all)
{
	if (((*command_cur))[(*all).i] == '$' && (*all).open_one == 0
		&& (ft_isalpha(((*command_cur))[(*all).i + 1])
		|| ((*command_cur))[(*all).i + 1] == '_'
		|| ((*command_cur))[(*all).i + 1] == '?'))
	{
		(*all).tmp = ft_strndup(*command_cur, (*all).i);
		(*all).tmp_2 = *command_cur;
		(*command_cur) = ft_strjoin((*all).tmp,
				serch_env((*command_cur) + (*all).i + 1,
					term, &((*all).size_name)));
		free((*all).tmp);
		(*all).tmp = ft_strdup((*all).tmp_2 + (*all).i + 1 + (*all).size_name);
		free((*all).tmp_2);
		(*all).tmp_2 = *command_cur;
		*command_cur = ft_strjoin((*all).tmp_2, (*all).tmp);
		free((*all).tmp);
		free((*all).tmp_2);
	}
}

void	pars_env_elem(t_terminal *term, char **command_cur)
{
	t_pars_env_elem	all;

	all.i = 0;
	all.open_one = 0;
	all.open_two = 0;
	while (((*command_cur))[all.i] != '\0')
	{
		pars_env_elem_utils(command_cur, term, &all);
		if (((*command_cur))[all.i] == '\'' && all.open_one == 1)
			all.open_one = 0;
		else if (((*command_cur))[all.i] == '\'' && all.open_two == 0)
			all.open_one = 1;
		if (((*command_cur))[all.i] == '"' && all.open_two == 1)
			all.open_two = 0;
		else if (((*command_cur))[all.i] == '"')
			all.open_two = 1;
		if (((*command_cur))[all.i] == '\0')
			break ;
		all.i++;
	}
}

char	*get_name_file_and_fd(t_terminal *term,
	char *start_name_file, char *tmp, int *i)
{
	int		j;
	int		size_name;
	char	*name;

	j = 0;
	size_name = 0;
	while (tmp[*i] != '\0' && tmp[*i] != '>'
		&& tmp[*i] != '<' && tmp[*i] != ' ')
	{
		size_name++;
		(*i)++;
	}
	name = (char *)malloc(sizeof(char) * (size_name + 1));
	if (name == NULL)
		print_error(NULL, strerror(errno), 0, term);
	while (j != size_name)
	{
		name[j] = start_name_file[j];
		j++;
	}
	name[j] = '\0';
	return (name);
}
