/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 15:14:14 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/16 20:16:14 by gparsnip         ###   ########.fr       */
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

int	ligic_cavichki_2(char *command)
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

int	count_char(char *command, int count)
{
	int	i;
	int	j;
	int	quotes;
	int	g;

	i = 0;
	g = 0;
	quotes = 0;
	j = 0;
	while (count != 0)
	{
		count_bad(command, &i);
		count--;
	}
	while (command[i] == ' ')
		i++;
	j = i;
	count_bad(command, &j);
	g = i;
	while (g != j)
	{
		if (command[j] == '\'')
		{
			g++;
			while (command[g] != '\'')
				g++;
			quotes += 2;
		}
		if (command[j] == '"')
		{
			g++;
			while (command[g] != '"')
				g++;
			quotes += 2;
		}
		g++;
	}
	j = j - quotes;
	return (j - i);
}

void	char_record(char **a, int count, char *command)
{
	int	i;
	int	j;
	int	g;

	i = 0;
	j = 0;
	g = 0;
	while (count != 0)
	{
		count_bad(command, &i);
		count--;
	}
	while (command[i] == ' ')
		i++;
	j = i;
	count_bad(command, &j);
	while (i != j)
	{
		if (command[i] == '\'')
		{
			i++;
			while (command[i] != '\'')
			{
				(*a)[g] = command[i];
				i++;
				g++;
			}
			i++;
		}
		else if (command[i] == '"')
		{
			i++;
			while (command[i] != '"')
			{
				(*a)[g] = command[i];
				i++;
				g++;
			}
			i++;
		}
		else
		{
			(*a)[g] = command[i];
			i++;
			g++;
		}
	}
	(*a)[g] = '\0';
}

int	pars_quotes(char **command, t_terminal *term, char ****command_pipe, int i)
{
	int	size;
	int	count;

	size = 0;
	count = 0;
	if ((size = ligic_cavichki_2(*command)) == -1) //А ты точно не хуету ввел?
	{
		ft_putstr_fd(*command, term->fd.error);
		ft_putstr_fd(": ", term->fd.error);
		ft_putstr_fd("error syntax", term->fd.error);
		ft_putstr_fd("\n", term->fd.error);
		return (1);
	}
	(*command_pipe)[i] = (char **)malloc(sizeof(char **) * (size + 1));
	if ((*command_pipe)[i] == NULL)
		print_error(NULL, strerror(errno), 0, term);
	(*command_pipe)[i][size] = NULL;
	while (size != 0)
	{
		(*command_pipe)[i][count] = (char *)malloc(count_char(*command, count) + 1);
		if ((*command_pipe)[i][count] == NULL)
			print_error(NULL, strerror(errno), 0, term);
		char_record(&((*command_pipe)[i][count]), count, *command);
		size--;
		count++;
	}
	return (0);
}

// Обрезка пробелов в строке и подсчет команд между пайпами
int ft_size_matrix_and_trim(char **matrix, char *c)
{
	int i;
	char *tmp;

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
	return (i);
}

// Поиск глобальных переменных
char	*serch_env(char *name, t_terminal *term, int *i)
{
	t_list_env *tmp;
	int j;

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
					update_variable_env(term, NULL, "_");
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

void pars_env_elem(t_terminal *term, char **command_cur) // Замена глобальных переменных на их значения
{
	int i;
	char *tmp;
	char *tmp_2;
	int open_one;//открытие '
	int open_two;//открытие "
	int size_name;

	i = 0;
	open_one = 0;
	open_two = 0;
	while (((*command_cur))[i] != '\0')
	{
	    if (((*command_cur))[i] == '$' && open_one == 0 && (ft_isalpha(((*command_cur))[i + 1]) || ((*command_cur))[i + 1] == '_'))
		{
			tmp = ft_strndup(*command_cur, i);
			tmp_2 = *command_cur;
			(*command_cur) = ft_strjoin(tmp, serch_env((*command_cur) + i + 1, term, &size_name));
			free(tmp);
			tmp = ft_strdup(tmp_2 + i + 1 + size_name);
			free(tmp_2);
			tmp_2 = *command_cur;
			*command_cur = ft_strjoin(tmp_2, tmp);
			free(tmp);
			free(tmp_2);
		}
		if (((*command_cur))[i] == '\'' && open_one == 1)
		    open_one = 0;
		else if (((*command_cur))[i] == '\'' && open_two == 0)
		    open_one = 1;
		if (((*command_cur))[i] == '"' && open_two == 1)
		    open_two = 0;
		else if (((*command_cur))[i] == '"')
		    open_two = 1;
		if (((*command_cur))[i] == '\0')
			break ;
		i++;
	}
}

char *get_name_file_and_fd(t_terminal *term, char *start_name_file, char *tmp, int *i)
{
	int j;
	int size_name;
	char *name;

	j = 0;
	size_name = 0;
	while (tmp[*i] != '\0' && tmp[*i] != '>' && tmp[*i] != '<' && tmp[*i] != ' ')
	{
		size_name++;
		(*i)++;
	}
	name = (char*)malloc(sizeof(char) * (size_name + 1));
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

int is_input_or_output(t_terminal *term, char *tmp, int *i)
{
	int count;
	char *start_name_file;
	char *name;
	int *fd;
	char c;

	count = 0;
	if (term->fd.out != 0 && term->fd.out != -1 && term->fd.out != 1 && term->fd.out != 2)
		close(term->fd.out);
	if (term->fd.in != 0 && term->fd.in != -1 && term->fd.in != 1 && term->fd.in != 2)
		close(term->fd.in);
	if (tmp[*i] == '>')
		fd = &(term->fd.out);
	else
		fd = &(term->fd.in);
	c = tmp[*i];
	while (tmp[*i] == '>' || tmp[*i] == '<' || tmp[*i] == ' ')
	{
		if (tmp[*i] == '>' || tmp[*i] == '<')
			count++;
		if ((tmp[*i] == '<' && count >= 2) || (tmp[*i] == '>' && count > 2))
		{
			ft_putstr_fd(tmp + *i - count, term->fd.error);
			ft_putstr_fd(": error syntax\n", term->fd.error);
			return (1);
		}
		(*i)++;
	}
	start_name_file = tmp + *i;
	name = get_name_file_and_fd(term, start_name_file, tmp, i);
	if (name != NULL)
	{
		if (c == '>')
		{
			*fd = open(name, O_CREAT | O_EXCL | O_RDWR, S_IRWXU);
			if (*fd == -1)
			{
				if (count == 2)
					*fd = open(name, O_APPEND | O_RDWR);
				else
					*fd = open(name, O_TRUNC | O_RDWR);
			}
			free(name);
		}
		else
			*fd = open(name, O_RDWR);
		if (*fd == -1)
		{
			ft_putstr_fd(name, term->fd.error);
			ft_putstr_fd(": ", term->fd.error);
			ft_putstr_fd(strerror(errno), term->fd.error);
			ft_putstr_fd("\n", term->fd.error);
			return (1);
		}
	}
	return (0);
}

int par_std_out(t_terminal *term, char **tmp) // Парсинг перенаправления вывода
{
	int i;
	int start;
	int end;
	char *new_tmp;
	char *fre;

	i = 0;
	new_tmp = NULL;
	while ((*tmp)[i] != '\0')
	{
		if ((*tmp)[i] == '>' || (*tmp)[i] == '<')
		{
			start = i;
			while ((*tmp)[start - 1] == ' ')
				start--;
			if (is_input_or_output(term, *tmp, &i))
				return (1);
			end = i;
			while ((*tmp)[end] == ' ')
			    end++;
			if (new_tmp != NULL)
				free(new_tmp);
			new_tmp = ft_strndup(*tmp, start);
			fre = new_tmp;
			new_tmp = ft_strjoin(new_tmp, (*tmp) + end);
			free(fre);
			free((*tmp));
			(*tmp) = ft_strdup(new_tmp);
			i = 0;
		}
		i++;
	}
	return (0);
}

void par_multi_cammand(t_terminal *term) // Проверка ;
{
	int pos;
	char *tmp;

	if (count_symbol_str(term->line, ';') != 0)
	{
		pos = ft_strclen(term->line, ';');
		tmp = ft_strndup(term->line, pos);
		free(term->line);
		term->line = tmp;
		term->flag.error = 1;
	}
}

int pre_pars(t_terminal *term, char ****command_pipe) // Главная функция парсера
{
	int size;
	int i;
	char **tmp;
	int ret;

	i = 0;
	ret = 1;
	par_multi_cammand(term);
	tmp = ft_split(term->line, '|');
	size = ft_size_matrix_and_trim(tmp, " ");
	*command_pipe = (char ***)malloc(sizeof(char **) * (size + 1));
	if (*command_pipe == NULL)
		print_error(NULL, strerror(errno), 0, term);
	while (i != size)
	{
		if (par_std_out(term, &tmp[i]))//определение потока вывода если он первый
		{
			(*command_pipe)[i] = NULL;
			ret = 0;
		}
		if (!ft_strncmp(tmp[i], "export", 6))
		{
			term->flag.export = 2;
			if (count_symbol_str(tmp[i], '$') != 0)
				term->flag.export = 1;
		}
		pars_env_elem(term, &tmp[i]);			//ДолларЧееек
		//if (pars_cavichki(&tmp[i], term))		// Чавички надо?
			//ret = 0;
		if (pars_quotes(&tmp[i], term, command_pipe, i))
		{
			(*command_pipe)[i] = NULL;
			ret = 0;
		}								//он дальше не очень использовался и возникала сега
		//par_std_out(); //определение потока вывода
		//(*command_pipe)[i] = ft_split(tmp[i], ' ');
		//printf("%s", (*command_pipe)[i]);
		free(tmp[i]);
		i++;
	}
	free(tmp);
	(*command_pipe)[i] = NULL;
	return (ret);
}
