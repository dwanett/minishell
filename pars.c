/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 15:14:14 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/17 17:46:19 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	logic_quotes(char *command, int *i, char a, int *size)
{
	if (a == ' ')
	{
		while (command[*i] == a)
			(*i)++;
		(*size)++;
	}
	else
	{
		(*i)++;
		while (command[*i] != a)
		{
			if (command[*i] == '\0')
				return (-1);
			(*i)++;
		}
	}
	return (0);
}

int	ligic_quost_2(char *command)
{
	int	size;
	int	i;
	int	flag;

	i = 0;
	size = 1;
	flag = 0;
	while (command[i] != '\0')
	{
		if (command[i] == ' ')
			flag = logic_quotes(command, &i, ' ', &size);
		if (command[i] == '\'')
			flag = logic_quotes(command, &i, '\'', &size);
		if (command[i] == '"')
			flag = logic_quotes(command, &i, '"', &size);
		if (flag == -1)
			return (-1);
		i++;
	}
	return (size);
}

void	count_bad(char *command, int *i)
{
	int	size;

	size = 0;
	if (command[*i] == ' ')
	{
		while (command[*i] == ' ')
			*i = *i + 1;
	}
	while (command[*i] != '\0' && command[*i] != ' ')
	{
		if (command[*i] == '\'')
			logic_quotes(command, i, '\'', &size);
		if (command[*i] == '"')
			logic_quotes(command, i, '"', &size);
		(*i)++;
	}
}

void	count_char_utils(char *command, int *quotes, t_char_record *all)
{
	if (command[(*all).j] == '\'')
	{
		(*all).g++;
		while (command[(*all).g] != '\'')
			(*all).g++;
		*quotes += 2;
	}
	if (command[(*all).j] == '"')
	{
		(*all).g++;
		while (command[(*all).g] != '"')
			(*all).g++;
		*quotes += 2;
	}
	(*all).g++;
}

int	count_char(char *command, int count)
{
	t_char_record	all;
	int				quotes;

	all.i = 0;
	all.g = 0;
	quotes = 0;
	all.j = 0;
	while (count != 0)
	{
		count_bad(command, &(all.i));
		count--;
	}
	while (command[all.i] == ' ')
		all.i++;
	all.j = all.i;
	count_bad(command, &(all.j));
	all.g = all.i;
	while (all.g != all.j)
		count_char_utils(command, &quotes, &all);
	all.j = all.j - quotes;
	return (all.j - all.i);
}

void	char_record_utils(char **a,
	char *command, t_char_record *all, char symb)
{
	if (symb != '\0')
	{
		(*all).i++;
		while (command[(*all).i] != symb)
		{
			(*a)[(*all).g] = command[(*all).i];
			(*all).i++;
			(*all).g++;
		}
		(*all).i++;
	}
	else
	{
		(*a)[(*all).g] = command[(*all).i];
		(*all).i++;
		(*all).g++;
	}
}

void	char_record(char **a, int count, char *command)
{
	t_char_record	all;

	all.i = 0;
	all.j = 0;
	all.g = 0;
	while (count != 0)
	{
		count_bad(command, &(all.i));
		count--;
	}
	while (command[all.i] == ' ')
		all.i++;
	all.j = all.i;
	count_bad(command, &(all.j));
	while (all.i != all.j)
	{
		if (command[all.i] == '\'')
			char_record_utils(a, command, &all, '\'');
		else if (command[all.i] == '"')
			char_record_utils(a, command, &all, '"');
		else
			char_record_utils(a, command, &all, '\0');
	}
	(*a)[all.g] = '\0';
}

int	pars_quotes(char **command, t_terminal *term, char ****command_pipe, int i)
{
	int	size;
	int	count;

	size = ligic_quost_2(*command);
	count = 0;
	if (size == -1)
	{
		print_error(*command, "error syntax", -1, term);
		return (1);
	}
	(*command_pipe)[i] = (char **)malloc(sizeof(char **) * (size + 1));
	if ((*command_pipe)[i] == NULL)
		print_error(NULL, strerror(errno), 0, term);
	(*command_pipe)[i][size] = NULL;
	while (size != 0)
	{
		(*command_pipe)[i][count] = (char *)malloc
			(count_char(*command, count) + 1);
		if ((*command_pipe)[i][count] == NULL)
			print_error(NULL, strerror(errno), 0, term);
		char_record(&((*command_pipe)[i][count]), count, *command);
		size--;
		count++;
	}
	return (0);
}

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

void pars_env_elem(t_terminal *term, char **command_cur)
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

char *get_name_file_and_fd(t_terminal *term,
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

int	all_name_null(t_terminal *term, t_input_or_output *all)
{
	if ((*all).c == '>')
	{
		*((*all).fd) = open((*all).name, O_CREAT | O_EXCL | O_RDWR, S_IRWXU);
		if (*((*all).fd) == -1)
		{
			if ((*all).count == 2)
				*((*all).fd) = open((*all).name, O_APPEND | O_RDWR);
			else
				*((*all).fd) = open((*all).name, O_TRUNC | O_RDWR);
		}
		free((*all).name);
	}
	else
		*((*all).fd) = open((*all).name, O_RDWR);
	if (*((*all).fd) == -1)
	{
		print_error((*all).name, strerror(errno), -1, term);
		return (1);
	}
	return (0);
}

void	is_input_or_output_init(t_input_or_output *all,
	t_terminal *term, char *tmp, int *i)
{
	(*all).count = 0;
	if (term->fd.out != 0 && term->fd.out != -1
		&& term->fd.out != 1 && term->fd.out != 2)
		close(term->fd.out);
	if (term->fd.in != 0 && term->fd.in != -1
		&& term->fd.in != 1 && term->fd.in != 2)
		close(term->fd.in);
	if (tmp[*i] == '>')
		(*all).fd = &(term->fd.out);
	else
		(*all).fd = &(term->fd.in);
	(*all).c = tmp[*i];
}

int is_input_or_output(t_terminal *term, char *tmp, int *i)
{
	t_input_or_output	all;

	is_input_or_output_init(&all, term, tmp, i);
	while (tmp[*i] == '>' || tmp[*i] == '<' || tmp[*i] == ' ')
	{
		if (tmp[*i] == '>' || tmp[*i] == '<')
			all.count++;
		if ((tmp[*i] == '<' && all.count >= 2)
			|| (tmp[*i] == '>' && all.count > 2))
		{
			ft_putstr_fd(tmp + *i - all.count, term->fd.error);
			ft_putstr_fd(": error syntax\n", term->fd.error);
			return (1);
		}
		(*i)++;
	}
	all.start_name_file = tmp + *i;
	all.name = get_name_file_and_fd(term, all.start_name_file, tmp, i);
	if (all.name != NULL)
		if (all_name_null(term, &all) == 1)
			return (1);
	return (0);
}

void	init_tmp_com(t_info_command	**tmp_com, t_terminal *term,
	t_info_command **last_elem, t_info_command **command_cur)
{
	(*tmp_com)->fd.in = term->fd.in;
	(*tmp_com)->fd.out = term->fd.out;
	(*tmp_com)->fd.error = term->fd.error;
	(*tmp_com)->fd.history = term->fd.history;
	term->fd.in = STDIN;
	term->fd.out = STDOUT;
	term->fd.error = STDERROR;
	if ((*command_cur) == NULL)
	{
		(*command_cur) = (*tmp_com);
		(*last_elem) = (*command_cur);
	}
	else
	{
		(*last_elem)->next = (*tmp_com);
		(*last_elem) = (*last_elem)->next;
	}
}

int	par_std_out_utils(t_start_end *all, char **tmp, t_terminal *term)
{
	(*all).start = (*all).i;
	while ((*tmp)[(*all).start - 1] == ' ')
		(*all).start--;
	if (is_input_or_output(term, *tmp, &((*all).i)))
		return (1);
	(*all).end = (*all).i;
	while ((*tmp)[(*all).end] == ' ')
		(*all).end++;
	if ((*all).new_tmp != NULL)
		free((*all).new_tmp);
	(*all).new_tmp = ft_strndup(*tmp, (*all).start);
	(*all).fre = (*all).new_tmp;
	(*all).new_tmp = ft_strjoin((*all).new_tmp, (*tmp) + (*all).end);
	return (0);
}

int par_std_out(t_terminal *term, char **tmp,
	t_info_command **command_cur, t_info_command **last_elem)
{
	t_start_end		all;
	t_info_command	*tmp_com;

	all.i = 0;
	all.new_tmp = NULL;
	tmp_com = (t_info_command *)malloc(sizeof(t_info_command));
	if (tmp_com == NULL)
		print_error(NULL, strerror(errno), 0, term);
	tmp_com->next = NULL;
	while ((*tmp)[all.i] != '\0')
	{
		if ((*tmp)[all.i] == '>' || (*tmp)[all.i] == '<')
		{
			if (par_std_out_utils(&all, tmp, term) == 1)
				return (1);
			free(all.fre);
			free((*tmp));
			(*tmp) = ft_strdup(all.new_tmp);
			free(all.new_tmp);
			all.i = 0;
		}
		all.i++;
	}
	init_tmp_com(&tmp_com, term, last_elem, command_cur);
	return (0);
}

void par_multi_cammand(t_terminal *term)
{
	int		pos;
	char	*tmp;

	if (count_symbol_str(term->line, ';') != 0)
	{
		pos = ft_strclen(term->line, ';');
		tmp = ft_strndup(term->line, pos);
		free(term->line);
		term->line = tmp;
		term->flag.error = 1;
	}
}

void	pre_pars_utils(t_terminal *term, int i, char **tmp)
{
	if (!ft_strncmp(tmp[i], "export", 6))
	{
		term->flag.export = 2;
		if (count_symbol_str(tmp[i], '$') != 0)
			term->flag.export = 1;
	}
}

void	pre_pars_init(int *ret,
	t_terminal *term, t_info_command **command_cur, char ***tmp)
{
	(*ret) = 1;
	par_multi_cammand(term);
	(*command_cur) = NULL;
	(*tmp) = ft_split(term->line, '|');
}

void	command_pipe_null(char ****command_pipe, int i, int *ret)
{
	(*command_pipe)[i] = NULL;
	*ret = 0;
}

int pre_pars(t_terminal *term,
	char ****command_pipe, t_info_command **command_cur, int i)
{
	char			**tmp;
	int				size;
	int				ret;
	t_info_command	*last_elem;

	pre_pars_init(&ret, term, command_cur, &tmp);
	size = ft_size_matrix_and_trim(tmp, " ", term, command_pipe);
	while (++i != size)
	{
		if (par_std_out(term, &tmp[i], command_cur, &last_elem))
			command_pipe_null(command_pipe, i, &ret);
		pre_pars_utils(term, i, tmp);
		pars_env_elem(term, &tmp[i]);
		if (pars_quotes(&tmp[i], term, command_pipe, i))
			command_pipe_null(command_pipe, i, &ret);
		free(tmp[i]);
	}
	free(tmp);
	(*command_pipe)[i] = NULL;
	return (ret);
}
