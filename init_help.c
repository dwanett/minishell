/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/27 15:55:04 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/27 15:57:06 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_status_command_env(t_list_env **env, t_terminal *term)
{
	t_list_env	*tmp;

	tmp = (t_list_env *)malloc(sizeof(t_list_env));
	if (tmp == NULL)
		print_error(NULL, strerror(errno), 0, term);
	tmp->name = ft_strdup("?");
	tmp->line = ft_strdup("0");
	tmp->update_variable = NULL;
	tmp->tmp_variable = 1;
	tmp->next = *env;
	*env = tmp;
	term->status = tmp;
}

void	init_env_help(t_list_env **tmp, t_terminal *term, int i, char **envp)
{
	char	*itoa;

	if (!ft_strncmp((*tmp)->name, "SHLVL", 5))
	{
		itoa = ft_itoa(ft_atoi((*tmp)->line) + 1);
		free((*tmp)->line);
		(*tmp)->line = ft_strdup(itoa);
		free(itoa);
	}
	if (!ft_strcmp((*tmp)->name, "PATH"))
		term->path = (*tmp);
	if (!ft_strcmp((*tmp)->name, "HOME"))
		term->home = (*tmp);
	if (!ft_strcmp((*tmp)->name, "_"))
	{
		(*tmp)->update_variable = ft_strdup(ft_strrchr(envp[i], '/') - 1);
		term->update = (*tmp);
	}
}

void	init_env(t_list_env **env, char **envp, t_terminal *term, int i)
{
	t_list_env	*tmp;

	while (envp[i] != NULL)
		i++;
	*env = NULL;
	while (--i >= 0)
	{
		tmp = (t_list_env *)malloc(sizeof(t_list_env));
		if (tmp == NULL)
			print_error(NULL, strerror(errno), 0, term);
		tmp->name = ft_strndup(envp[i], ft_strclen(envp[i], '='));
		tmp->line = ft_strdup(envp[i] + ft_strclen(envp[i], '=') + 1);
		tmp->update_variable = NULL;
		tmp->tmp_variable = 0;
		init_env_help(&tmp, term, i, envp);
		tmp->next = *env;
		*env = tmp;
	}
	init_status_command_env(env, term);
}

void	init_env_for_next_process_help(int i,
		t_list_env *envp, t_terminal *term)
{
	char	*tmp;

	tmp = ft_strjoin(envp->name, "=");
	term->start_env[i] = tmp;
	tmp = ft_strjoin(tmp, envp->line);
	free(term->start_env[i]);
	term->start_env[i] = tmp;
}

void	init_env_for_next_process(t_terminal *term, t_list_env *envp)
{
	int			i;
	t_list_env	*tmp_env;

	i = 0;
	tmp_env = envp;
	while (tmp_env != NULL)
	{
		if (tmp_env->tmp_variable != 1)
			i++;
		tmp_env = tmp_env->next;
	}
	term->start_env = (char **)malloc(sizeof(char *) * (i + 1));
	if (term->start_env == NULL)
		print_error(NULL, strerror(errno), 0, term);
	i = 0;
	while (envp != NULL)
	{
		if (envp->tmp_variable != 1)
		{
			init_env_for_next_process_help(i, envp, term);
			i++;
		}
		envp = envp->next;
	}
	term->start_env[i] = NULL;
}
