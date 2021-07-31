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

int check_def_com(char *command) //Проверка /bin
{
	DIR *dir;
	struct dirent *dp;

	dir = opendir("/bin");
	dp = readdir(dir);
	while (dp != NULL)
	{
		if (!ft_strcmp(command, dp->d_name))
		{
			closedir(dir);
			return (0);
		}
		dp = readdir(dir);
	}
	closedir(dir);
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

void pars_def_command(char ***command, char *line) // Обработка команд /bin или не команд // Не работает с переменными среды
{
	char *tmp;
	pid_t pid;
	int l;
	int status;

	*command = ft_split(line, ' ');
	if (check_def_com(**command)) //это команда? (если есть файл в /bin - это команда)
	{
		ft_putstr_fd(line, 2);
		ft_putstr_fd(": command not found", 2);
		ft_putstr_fd("\n", 2);
		return ;
	}
	tmp = ft_strjoin("/bin/", **command);
	free(**command);
	**command = tmp;
	utils_for_civichki(command);
	pid = fork(); // создание потока для выполения команды
	if (pid == 0)
		l = execve(*command[0], *command, NULL);
	if (l == -1)
		exit(errno);
	waitpid(pid, &status, 0);
}

void ft_cd(char ***command, int i) // команда cd // Не работает без аргументов
{
	if (i >= 3)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return ;
	}
	if (chdir(*(*command + 1)) == -1)
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
	char *not_def_com[4];
	char **command;
	int i;
	int ret;

	i = 0;
	not_def_com[0] = "cd";
	not_def_com[1] = "export";
	not_def_com[2] = "unset";
	not_def_com[3] = "env";
	ret = check_not_def_com(line, not_def_com);
	if (ret != -1)									// проверка команд (они не дефолтные?)
		pars_not_def_command(&command, line, ret, not_def_com); // обработка не дефолтных команд
	else											// Они не дефолтные! И есть в папке /bin. Или это не команды.
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
	else if (ft_strcmp(term->line, "\0") && ft_strcmp(term->line, term->history_cmd->command)) // если строка не пустая
	{
		ft_add_history(term);
		add_history(term->line); //добавление истории для readline
	}
	command(term->line);	//функция обработки команд
}

int	main(int argc, char **argv, char **envp)
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
