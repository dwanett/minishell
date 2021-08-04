/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 15:28:39 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/02 15:28:39 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_env(t_terminal *term, int flag)
{
	t_list_env *tmp;

	tmp = term->env;
	while (tmp != NULL)
	{
		if (flag == 1)
		{
			ft_putstr_fd("declare -x ", term->fd.out); //есть еще кавычки //тестируй без аргументов команду export
			ft_putstr_fd(tmp->name, term->fd.out);
			ft_putstr_fd("=\"", term->fd.out);
			ft_putstr_fd(tmp->line, term->fd.out);
			ft_putstr_fd("\"", term->fd.out);
		}
		else
		{
			ft_putstr_fd(tmp->name, term->fd.out);
			ft_putstr_fd("=", term->fd.out);
			ft_putstr_fd(tmp->line, term->fd.out);
		}
		ft_putstr_fd("\n", term->fd.out);
		tmp = tmp->next;
	}
}

void del_element_env(char *elem, t_terminal *term) //Удаление переменной
{
	t_list_env *tmp;
	t_list_env *prev;

	tmp = term->env;
	prev = tmp;
	while (tmp != NULL)
	{
		if (!ft_strcmp(tmp->name, elem))
		{
			if (prev == term->env)
				term->env = term->env->next;
			else
			{
				prev->next = tmp->next;
				free(tmp->name);
				free(tmp->line);
				free(tmp);
				tmp = prev;
			}
			break;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int is_name(char *elem, int flag)
{
	int i;

	i = 0;
	while (elem[i] != '\0')
	{
		if (flag == 1 && elem[i] == '=')
			break;
		if (!ft_isalpha(elem[i]) && elem[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

void ft_unset(char ***command, t_terminal *term, int size_arg) // не всегда удаляет переменную (причина неизвестна)
{
	int i;

	i = 1;
	while (i != size_arg)
	{
		if (!is_name(*(*command + i), 0))
			del_element_env(*(*command + i), term); //Удаление переменной
		else
		{
			ft_putstr_fd("unset: ", term->fd.error);
			ft_putstr_fd(*(*command + i), term->fd.error);
			ft_putstr_fd(": ", term->fd.error);
			ft_putstr_fd("not a valid identifier", term->fd.error);
			ft_putstr_fd("\n", term->fd.error);
		}
		i++;
	}
}

int is_new_perem_export(char *peremen, t_list_env *env) //Проверка на дубликат переменной
{
	t_list_env *tmp;

	tmp = env;
	while (tmp != NULL)
	{
		if (ft_strncmp(tmp->line, peremen, ft_strclen(peremen, '=')))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int is_ravenstvo(char *peremen) //проверка на символ =
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (peremen[i] != '\0')
	{
		if (peremen[i] == '=')
			j = i;
		i++;
	}
	if (j != 0)
		return (1);
	return (0);
}

void ft_export(char ***command, t_terminal *term, int size_arg)
{
	t_list_env *new_env;
	t_list_env *tmp;
	int i;

	i = 1;
	tmp = term->env;
	if (!*(*command + 1)) // если нет аргументов
	{
		ft_env(term, 1);
		return;
	}
	while (i != size_arg)
	{
		term->env = tmp;
		if (is_ravenstvo(*(*command + i))) //есть ли равно?
		{
			if (!is_name(*(*command + i), 1)) //имя состоит из букв?
			{
				if (is_new_perem_export(*(*command + i),
										tmp))				//если такая переменная уже есть, то удалить ее
					del_element_env(*(*command + i), term); //Удаление переменной
				while (term->env && term->env->next != NULL)
					term->env = term->env->next;
				new_env = (t_list_env *)malloc(sizeof(t_list_env));
				new_env->name = ft_strndup(*(*command + i), ft_strclen(*(*command + i), '='));
				new_env->line = ft_strdup(*(*command + i) + ft_strclen(*(*command + i), '=') + 1);
				new_env->next = NULL;
				if (term->env)
					term->env->next = new_env;
				else
					tmp = new_env;
			}
			else
			{
				ft_putstr_fd("export: ", term->fd.error);
				ft_putstr_fd(*(*command + i), term->fd.error);
				ft_putstr_fd(": ", term->fd.error);
				ft_putstr_fd("not a valid identifier", term->fd.error);
				ft_putstr_fd("\n", term->fd.error);
			}
		}
		i++;
	}
	term->env = tmp;
}
