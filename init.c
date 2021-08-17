/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:35:21 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/17 16:42:23 by dwanetta         ###   ########.fr       */
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
		if (!ft_strcmp(tmp->name, "PATH"))
			term->path = tmp;
		if (!ft_strcmp(tmp->name, "_"))
		{
			tmp->update_variable = ft_strdup(ft_strrchr(envp[i], '/') - 1);
			term->update = tmp;
		}
		tmp->next = *env;
		*env = tmp;
	}
	init_status_command_env(env, term);
}

void	init_env_for_next_process(t_terminal *term, char **envp, int j)
{
	int		size_env;
	char	*tmp;
	char	*itoa;

	size_env = 0;
	while (envp[size_env] != NULL)
		size_env++;
	term->start_env = (char **)malloc(sizeof(char *) * (size_env + 1));
	if (term->start_env == NULL)
		print_error(NULL, strerror(errno), 0, term);
	while (++j != size_env)
	{
		if (ft_strncmp(envp[j], "SHLVL", 5))
			term->start_env[j] = ft_strdup(envp[j]);
		else
		{
			tmp = ft_strndup(envp[j], ft_strclen(envp[j], '=') + 1);
			itoa = ft_itoa(ft_atoi(ft_strchr(envp[j], '=') + 1) + 1);
			term->start_env[j] = ft_strjoin(tmp, itoa);
			free(tmp);
			free(itoa);
		}
	}
	term->start_env[j] = NULL;
}

void	init_t_teminal(t_terminal *term, int argc, char **argv, char **envp)
{
	term->update = NULL;
	init_env_for_next_process(term, envp, -1);
	init_env(&term->env, term->start_env, term, 0);
	(void)argc;
	(void)argv;
	term->fd.history = -1;
	term->flag.export = 0;
	term->flag.def_com = 0;
	term->flag.error = 0;
	term->line = NULL;
	term->history_cmd = NULL;
	term->fd.in = STDIN;
	term->fd.out = STDOUT;
	term->fd.error = STDERROR;
	term->not_def_command[0] = "cd";
	term->not_def_command[1] = "export";
	term->not_def_command[2] = "unset";
	term->not_def_command[3] = "env";
	read_file_history(term);
}

int	init_term_fd(t_terminal *term)
{
	if (term->fd.in != STDIN)
	{
		close(term->fd.in);
		term->fd.in = STDIN;
	}
	if (term->fd.out != STDOUT)
	{
		close(term->fd.out);
		term->fd.out = STDOUT;
	}
	if (term->fd.error != STDERROR)
	{
		close(term->fd.error);
		term->fd.error = STDERROR;
	}
	return (1);
}
