/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwanetta <dwanetta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 23:57:29 by dwanetta          #+#    #+#             */
/*   Updated: 2021/07/30 20:05:21 by dwanetta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_not_def_com(char *line, char **not_def_com) //Проверка команды по списку не дефолтных
{
	int i;

	i = 0;
	while (i != 4)
	{
		if (!ft_strncmp(not_def_com[i], line, ft_strlen(not_def_com[i])))
			return (i);
		i++;
	}
	return (-1);
}

int check_def_com(char *command, char **path) //Проверка /bin
{
	DIR *dir;
	struct dirent *dp;
	char **patch_env;
	int i;
	int j;

	i = 0;
	j = 0;
	patch_env = ft_split(getenv("PATH"), ':'); // получение путей из переменной среды PATH
	while (patch_env[i] != NULL)
	{
		dir = opendir(patch_env[i]); // открываем каждую директорию и проверяем наличие команды
		dp = readdir(dir);
		while (dp != NULL)
		{
			if (!ft_strcmp(command, dp->d_name))
			{
				closedir(dir);
				*path = patch_env[i];
				while (patch_env[j] != NULL)
				{
					if (i != j)
						free(patch_env[j]);
					j++;
				}
				free(patch_env);
				return (0);
			}
			dp = readdir(dir);
		}
		closedir(dir);
		i++;
	}
	return (1);
}

void utils_for_civichki(char ***command)
{
	char **tmp2;

	if (*(*command + 1) != NULL)
	{
		tmp2 = ft_split(*(*command + 1), '"');
		free(*(*command + 1));
		*(*command + 1) = *tmp2;
	}
}

int is_path(const char *command)//Это путь?
{
	int i;

	i = 0;
	while (command[i] != '\0')
	{
		if (command[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

void pars_def_command(char ***command, char *line) // Обработка команд /bin или не команд // Не работает с переменными среды
{
	char *tmp;
	pid_t pid;
	int l;
	int status;
	char *path;

	*command = ft_split(line, ' ');
	if (!is_path(**command)) // Если это не путь то
	{
		if (check_def_com(**command, &path)) //это команда? (если есть файл в /bin - это команда)
		{
			ft_putstr_fd(line, 2);
			ft_putstr_fd(": command not found", 2);
			ft_putstr_fd("\n", 2);
			return ;
		}
		tmp = ft_strjoin(path, "/");
		free(path);
		path = tmp;
		tmp = ft_strjoin(path, **command);
		free(**command);
		**command = tmp;
		free(path);
	}
	utils_for_civichki(command);
	pid = fork(); // создание потока для выполения команды
	if (pid == 0)
		l = execve(*command[0], *command, NULL);
	if (l == -1)
	{
		ft_putstr_fd(*command[0], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		exit(errno);
	}
	waitpid(pid, &status, 0);
}

void ft_cd(char ***command, int i) // команда cd
{
	int ret;

	if (i >= 3)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return ;
	}
	if (i == 1)
		ret = chdir(getenv("HOME"));
	else
		ret = chdir(*(*command + 1));
	if (ret == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(*(*command + 1), 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return ;
	}
}

void ft_env(t_terminal *term, int flag)
{
	t_list_env *tmp;

	tmp = term->env;
	while (tmp != NULL)
	{
		if (flag == 1)
		{
			ft_putstr_fd("declare -x ", 1); //есть еще кавычки //тестируй без аргументов команду export
		}
		ft_putstr_fd(tmp->line, 1);
		ft_putstr_fd("\n", 1);
		tmp = tmp->next;
	}
}

void del_element_env(char *elem, t_terminal *term)//Удаление переменной
{
	t_list_env *tmp;
	t_list_env *prev;

	tmp = term->env;
	prev = tmp;
	while (tmp != NULL)
	{
		if (!ft_strncmp(tmp->line, elem, ft_strclen(elem, '=')))
		{
			if (prev == term->env)
				term->env = term->env->next;
			else
			{
				prev->next = tmp->next;
				free(tmp->line);
				free(tmp);
				tmp = prev;
			}
			break ;
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
			break ;
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
			del_element_env(*(*command + i), term);//Удаление переменной
		else
		{
			ft_putstr_fd("unset: ", 2);
			ft_putstr_fd(*(*command + i), 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd("not a valid identifier", 2);
			ft_putstr_fd("\n", 2);
		}
		i++;
	}
}

int is_new_perem_export(char *peremen, t_list_env *env)//Проверка на дубликат переменной
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

int is_ravenstvo(char *peremen)//проверка на символ =
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
		return ;
	}
	while (i != size_arg)
	{
		term->env = tmp;
		if (is_ravenstvo(*(*command + i))) //есть ли равно?
		{
			if (!is_name(*(*command + i), 1)) //имя состоит из букв?
			{
				if (is_new_perem_export(*(*command + i),
						tmp)) //если такая переменная уже есть, то удалить ее
					del_element_env(*(*command + i), term);//Удаление переменной
				while (term->env && term->env->next != NULL)
					term->env = term->env->next;
				new_env = (t_list_env *) malloc(sizeof(t_list_env));
				new_env->line = ft_strdup(*(*command + i));
				new_env->next = NULL;
				if (term->env)
					term->env->next = new_env;
				else
					tmp = new_env;
			}
			else
			{
				ft_putstr_fd("export: ", 2);
				ft_putstr_fd(*(*command + i), 2);
				ft_putstr_fd(": ", 2);
				ft_putstr_fd("not a valid identifier", 2);
				ft_putstr_fd("\n", 2);
			}
		}
		i++;
	}
	term->env = tmp;
}

void pars_not_def_command(char ***command, t_terminal *term, int i, char **not_def_com) // Обработка не дефолтных
{
	char **tmp;
	int j;
	int size_arg;

	j = 0;
	size_arg = 0;
	*command = ft_split(term->line, ' ');
	utils_for_civichki(command);
	tmp = *command;
	while (*(*command + size_arg) != NULL)
		size_arg++;
	while (tmp != NULL && *tmp != NULL)
	{
		tmp++;
		j++;
	}
	if (!ft_strncmp(not_def_com[i], "cd", 2))
		ft_cd(command, j);
	else if (!ft_strcmp(not_def_com[i], "env"))
		ft_env(term, 0);
	else if (!ft_strcmp(not_def_com[i], "export"))
		ft_export(command, term, size_arg);
	else if (!ft_strcmp(not_def_com[i], "unset"))
		ft_unset(command, term, size_arg);
}

void command(t_terminal *term)
{
	char *not_def_com[4];
	char **command;
	int i;
	int ret;

	i = 0;
	not_def_com[0] = "cd"; //возможно эти команды надо делать отдельным процессом, но хз
	not_def_com[1] = "export";
	not_def_com[2] = "unset";
	not_def_com[3] = "env";
	ret = check_not_def_com(term->line, not_def_com);
	if (ret != -1)									// проверка команд (они не дефолтные?)
		pars_not_def_command(&command, term, ret, not_def_com); // обработка не дефолтных команд
	else									// Они не дефолтные! И есть в папке /bin. Или это не команды.
		pars_def_command(&command, term->line);
	while (command[i] != NULL)
	{
		free(command[i]);
		i++;
	}
	free(command);
}

void teminal(t_terminal *term) //чтение строк терминала
{
	term->line = readline("minishell$ ");
	if (term->line == NULL || !ft_strncmp(term->line, "exit", 4)) // НАДО ПЕРЕНЕСТИ В КОМАНДЫ И ПРОВЕРИТЬ CASE exitr || exit r
		ft_exit(term);
	else if (ft_strcmp(term->line, "\0")) // если строка не пустая
	{
		if (term->history_cmd)
		{
			if (ft_strcmp(term->line, term->history_cmd->command))
			{
				ft_add_history(term);
				add_history(term->line);   //добавление истории для readline
			}
		}
		else
		{
			ft_add_history(term);
			add_history(term->line);
		}
		command(term); //функция обработки команд
	}
}

void init_env(t_list_env **env, char **envp)
{
	t_list_env *tmp;
	int i;

	i = 0;
	while (envp[i] != NULL)
		i++;
	*env = NULL;
	i--;
	while(i >= 0)
	{
		tmp = (t_list_env*)malloc(sizeof(t_list_env));
		if (tmp == NULL)
		{
			ft_putstr_fd(strerror(errno), 2);
			exit(errno);
		}
		tmp->line = ft_strdup(envp[i]);
		if (*env == NULL)
			tmp->next = NULL;
		else
			tmp->next = *env;
		*env = tmp;
		i--;
	}
}

int main(int argc, char **argv, char **envp)
{
	t_terminal term;

	init_env(&term.env, envp);
	(void)argc;
	(void)argv;
	signal(SIGTSTP, SIG_IGN);
	signal(SIGINT, ft_print_n);
	term.fd_history = -1;
	term.line = NULL;
	term.history_cmd = NULL;
	read_file_history(&term);
	if (term.fd_history != -1)
		close(term.fd_history);
	term.fd_history = -1;
	while (1)
		teminal(&term);
	close(term.fd_history);
	return (0);
}
// ВСЕ КОМАНДЫ РАБОТАЮТ. НАДО ТЕСТИРОВАТЬ РАЗНЫЕ КЕЙСЫ. ДОБАВИТЬ ОБРАБОТКУ ВСЯКИХ СИМВОЛОВ ЗАДАЧА №1
// ЕСТЬ ЕБУЧИЙ ЛИК, КОТОРЫЙ Я НЕ МОГУ ПОФИКСТИЬ !!!!!!!!!!!!!!!!!!!!!!!!!!!
// НЕ РАБОТАЕТ ПЕРЕНАПРПВЛЕНИЕ ВВОДА И ВЫВОДА	ЗАДАЧА №2
// НЕ РАБОТАЮТ ПАЙПЫ							ЗАДАЧА №3