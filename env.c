/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 15:28:39 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/17 17:36:04 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_variable_env(t_terminal *term, char *path_com,
		char *last_arg, t_list_env *tmp)
{
	if (term->update == NULL)
	{
		tmp = (t_list_env *)malloc(sizeof(t_list_env));
		if (tmp == NULL)
			print_error(NULL, strerror(errno), 0, term);
		if (path_com != NULL)
			tmp->line = ft_strdup(path_com);
		else
			tmp->line = ft_strdup("/usr/bin/env");
		tmp->update_variable = ft_strdup_quotes(last_arg);
		tmp->name = ft_strdup("_");
		tmp->next = term->env;
		term->env = tmp;
		term->update = tmp;
	}
	else
	{
		if (path_com != NULL)
		{
			free(term->update->line);
			term->update->line = ft_strdup(path_com);
		}
		free(term->update->update_variable);
		term->update->update_variable = ft_strdup_quotes(last_arg);
	}
}

char	*is_valid_env_arg(char **command)
{
	int	i;

	i = 1;
	while (command[i] != NULL)
	{
		if (count_symbol_str(command[i], '=') == 0)
		{
			if (*command[i] == '\0')
				return (NULL);
			open(command[i], O_RDWR);
			return (command[i]);
		}
		i++;
	}
	return (NULL);
}

void	ft_env_help(t_list_env *tmp, t_terminal *term, int flag)
{
	while (tmp != NULL)
	{
		if (flag == 1)
		{
			update_variable_env(term, "/usr/bin/export", "export", NULL);
			if (ft_strcmp(tmp->name, "_") && tmp->tmp_variable != 1)
			{
				ft_putstr_fd("declare -x ", term->fd.out);
				ft_putstr_fd(tmp->name, term->fd.out);
				if (tmp->is_initialized == 1)
				{
					ft_putstr_fd("=\"", term->fd.out);
					ft_putstr_fd(tmp->line, term->fd.out);
					ft_putstr_fd("\"", term->fd.out);
				}
				ft_putstr_fd("\n", term->fd.out);
			}
		}
		else if (tmp->tmp_variable == 0 && tmp->is_initialized == 1)
		{
			ft_putstr_fd(tmp->name, term->fd.out);
			ft_putstr_fd("=", term->fd.out);
			ft_putstr_fd(tmp->line, term->fd.out);
			ft_putstr_fd("\n", term->fd.out);
		}
		tmp = tmp->next;
	}
}

void	ft_env_init(t_list_env **tmp,
	t_terminal *term, char **check_error, int *i)
{
	*tmp = term->env;
	*i = 1;
	free(term->status->line);
	term->status->line = ft_strdup("0");
	*check_error = NULL;
}

void	ft_env(t_terminal *term, int flag, char ***command)
{
	t_list_env	*tmp;
	char		*check_error;
	int			i;

	ft_env_init(&tmp, term, &check_error, &i);
	update_variable_env(term, "/usr/bin/env", "env", NULL);
	if (command != NULL)
		check_error = is_valid_env_arg((*command));
	if (check_error == NULL || flag == 1)
	{
		ft_env_help(tmp, term, flag);
		if (flag == 0)
		{
			while ((*command)[i] != NULL)
			{
				ft_putstr_fd((*command)[i], term->fd.out);
				ft_putstr_fd("\n", term->fd.out);
				i++;
			}
		}
	}
	else if (errno != 0)
		print_error(check_error, strerror(errno), 1, term);
}
