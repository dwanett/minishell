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
	while (i != 1)
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

int is_path(char *command)
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

	ret = 0;
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

void peremennie_sredi(char ***command, int flag)
{
	if (flag == 0)
	{
		//bash -c export
	}
	else
	{
		//bash -c unset
	}
}

void pars_not_def_command(char ***command, char *line, int i, char **not_def_com) // Обработка не дефолтных
{
	char **tmp;
	int j;

	j = 0;
	*command = ft_split(line, ' ');
	utils_for_civichki(command);
	tmp = *command;
	while (tmp != NULL && *tmp != NULL)
	{
		tmp++;
		j++;
	}
	if (!ft_strncmp(not_def_com[i], "cd", 2))
		ft_cd(command, j);
	//else if (!ft_strcmp(not_def_com[i], "export"))
	//	peremennie_sredi(command, 0);
	//else if (!ft_strcmp(not_def_com[i], "unset"))
	//	peremennie_sredi(command, 0);
	//else if (!ft_strcmp(not_def_com[i], "env"))
		//bash -c env
}

void command(char *line)
{
	char *not_def_com[1];
	char **command;
	int i;
	int ret;

	i = 0;
	not_def_com[0] = "cd";
	//not_def_com[1] = "export";
	//not_def_com[2] = "unset";
	//not_def_com[3] = "env";
	ret = check_not_def_com(line, not_def_com);
	if (ret != -1)									// проверка команд (они не дефолтные?)
		pars_not_def_command(&command, line, ret, not_def_com); // обработка не дефолтных команд
	else									// Они не дефолтные! И есть в папке /bin. Или это не команды.
		pars_def_command(&command, line);
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
		command(term->line); //функция обработки команд
	}
}

int main(int argc, char **argv, char **envp)
{
	t_terminal term;

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
// НЕ РАБОТАЮТ КОМАНДЫ С ПРЕМЕННЫМИ СРЕДЫ		ЗАДАЧА 1
// НЕ РАБОТАЕТ ПЕРЕНАПРПВЛЕНИЕ ВВОДА И ВЫВОДА	ЗАДАЧА 2
// НЕ РАБОТАЮТ ПАЙПЫ							ЗАДАЧА 3