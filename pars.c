/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 15:14:14 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/10 19:15:29 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
int	ligic_cavichki(char *command) //Проверка правильности расстановки кавычек
{
	int ferst_pos;
	int last_pos;
	char c;
	int i;

	i = 0;
	while (command[i] != '\0')
	{
		ferst_pos = -1;
		last_pos = -1;
		if (command[i] == '\'' || command[i] == '"')
		{
			if (command[i] == '\'')
				c = '\'';
			else
				c = '"';
			ferst_pos = i;
		}
		if (ferst_pos != -1)
		{
			while (command[i] != '\0')
			{
				i++;
				if (command[i] == c)
				{
					last_pos = i;
					break;
				}
			}
		}
		if (ferst_pos != -1 && last_pos == -1)
			return (1);
		i++;
	}
	return (0);
}
*/
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
	(*command_pipe)[i][size] = NULL;
	while (size != 0)
	{
		(*command_pipe)[i][count] = (char *)malloc(count_char(*command, count) + 1);
		char_record(&((*command_pipe)[i][count]), count, *command);
		size--;
		count++;
	}
	return (0);
}

/*
int		pars_cavichki(char **command, t_terminal *term) // Удаление кавычек из строки
{
	int i;
	int j;
	char *tmp;
	int size;
	int count_one;
	int count_two;
	int flag;

	i = 0;
	j = 0;
	if (ligic_cavichki(*command)) //А ты точно не хуету ввел?
	{
		ft_putstr_fd(*command, term->fd.error);
		ft_putstr_fd(": ", term->fd.error);
		ft_putstr_fd("error syntax", term->fd.error);
		ft_putstr_fd("\n", term->fd.error);
		return (1);
	}
	count_one = count_symbol_str(*command, '\'');
	count_two = count_symbol_str(*command, '"');
	if (count_one % 2 != 0)
		count_one--;
	if (count_two % 2 != 0)
		count_two--;
	size = ft_strlen(*command) - count_one - count_two;
	tmp = (char *)malloc(sizeof(char) * size + 1);
	flag = 1;
	count_one = 0;
	count_two = 0;
	while ((*command)[i] != '\0')
	{
		if ((*command)[i] == '"' && count_one == 0)
		{
			if (count_two != 1)
				count_two++;
			else
				count_two = 0;
			flag = 2;
		}
		if ((*command)[i] == '\'' && count_two == 0)
		{
			if (count_one != 1)
				count_one++;
			else
				count_one = 0;
			flag = 1;
		}
		if (!(flag == 1 && (*command)[i] == '\'') && !(flag == 2 && (*command)[i] == '"'))
		{
			tmp[j] = (*command)[i];
			j++;
		}
		i++;
	} // Копи кавычек завершено
	tmp[j] = '\0';
	free(*command);
	*command = tmp;
	return (0);
}
*/

int ft_size_matrix_and_trim(char **matrix, char *c) // Обрезка пробелов в строке и подсчет команд между пайпами
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

char *serch_env(char *name, t_terminal *term, int *i) // Поиск глобальных переменных
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

//void par_std_out(t_terminal *term, char *tmp) // Парсинг перенаправления вывода
//
//	int i;

//	i = 0;
//	while (tmp[i] != '\0')
//	{
//		if (tmp[i] == '>' || tmp[i] == '<')
//		{
//			if (!ft_strncmp(tmp + i, ">>", 2))
//		}
//		i++;
//	}
//

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
	while (i != size)
	{
		//par_std_out(); //определение потока вывода если он первый
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
