/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:14:17 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/17 16:14:27 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Проверка команды по списку не дефолтных
int	check_not_def_com(char *line, char **not_def_com)
{
	int	i;

	i = 0;
	while (i != 5)
	{
		if (!ft_strncmp(not_def_com[i], line, ft_strlen(not_def_com[i])))
			return (i);
		i++;
	}
	return (-1);
}

void	check_def_com_help_free_path(int *j, char ***patch_env)
{
	while ((*patch_env)[*j] != NULL)
	{
		free((*patch_env)[*j]);
		(*j)++;
	}
	free((*patch_env));
}

int	check_def_com_help_help(char **path, char ***patch_env, int *j, int i)
{
	*path = ft_strdup((*patch_env)[i]);
	check_def_com_help_free_path(j, patch_env);
	return (1);
}

int	check_def_com_help(char *command, char **path, char ***patch_env, int *j)
{
	struct dirent	*dp;
	DIR				*dir;
	int				i;

	i = 0;
	while ((*patch_env) && (*patch_env)[i] != NULL)
	{
		dir = opendir((*patch_env)[i]);
		if (dir != NULL)
		{
			dp = readdir(dir);
			while (dp != NULL)
			{
				if (!ft_strcmp(command, dp->d_name))
				{
					closedir(dir);
					return (check_def_com_help_help(path, patch_env, j, i));
				}
				dp = readdir(dir);
			}
			closedir(dir);
		}
		i++;
	}
	return (0);
}

int	check_def_com(t_terminal *term, char *command, char **path)
{
	char			**patch_env;
	int				j;

	j = 0;
	patch_env = NULL;
	if (term->path)
		patch_env = ft_split(term->path->line, ':');
	if (check_def_com_help(command, path, &patch_env, &j))
		return (0);
	if (patch_env)
		check_def_com_help_free_path(&j, &patch_env);
	return (1);
}
