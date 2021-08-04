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

int	check_not_def_com(char *line, char **not_def_com) //Проверка команды по списку не дефолтных
{
	int	i;

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
	free(patch_env);
	return (1);
}

int count_symbol_str(const char *str, char c)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count);
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

int check_def_command(char ***command, t_terminal *term)
{
	char *path;
	char *tmp;

	if (!is_path(**command)) // Если это не путь то
	{
		if (check_def_com(**command, &path)) //это команда? (если есть файл в /bin - это команда)
		{
			ft_putstr_fd(*command[0], term->fd.error);
			ft_putstr_fd(": command not found", term->fd.error);
			ft_putstr_fd("\n", term->fd.error);
			return (0);
		}
		tmp = ft_strjoin(path, "/");
		free(path);
		path = tmp;
		tmp = ft_strjoin(path, **command);
		free(**command);
		**command = tmp;
		free(path);
	}
	return (1);
}

void pars_def_command(char ***command, t_terminal *term) // Обработка команд /bin или не команд // Не работает с переменными среды
{
	pid_t pid;
	int l;
	int status;

	l = 0;
	pid = fork(); // создание потока для выполения команды
	if (pid == 0)
		l = execve(*command[0], *command, NULL);
	if (l == -1)
	{
		ft_putstr_fd(*command[0], term->fd.error);
		ft_putstr_fd(": ", term->fd.error);
		ft_putstr_fd(strerror(errno), term->fd.error);
		ft_putstr_fd("\n", term->fd.error);
		exit(errno);
	}
	waitpid(pid, &status, 0);
}

void ft_cd(char ***command, int i, t_terminal *term) // команда cd
{
	int ret;

	if (i >= 3)
	{
		ft_putstr_fd("cd: too many arguments\n", term->fd.error);
		return ;
	}
	if (i == 1)
		ret = chdir(getenv("HOME"));
	else
		ret = chdir(*(*command + 1));
	if (ret == -1)
	{
		ft_putstr_fd("cd: ", term->fd.error);
		ft_putstr_fd(*(*command + 1), term->fd.error);
		ft_putstr_fd(": ", term->fd.error);
		ft_putstr_fd(strerror(errno), term->fd.error);
		ft_putstr_fd("\n", term->fd.error);
		return ;
	}
}

void pars_not_def_command(char ***command, t_terminal *term, int i) // Обработка не дефолтных
{
	char **tmp;
	int j;
	int size_arg;

	j = 0;
	size_arg = 0;
	tmp = *command;
	while (*(*command + size_arg) != NULL)
		size_arg++;
	while (tmp != NULL && *tmp != NULL)
	{
		tmp++;
		j++;
	}
	if (i == 0)
		ft_cd(command, j, term);
	else if (i == 1)
		ft_export(command, term, size_arg);
	else if (i == 2)
		ft_unset(command, term, size_arg);
	else if (i == 3)
		ft_env(term, 0);
}

void command(t_terminal *term)
{
	char ***command_pipe;
	char **command_cur;
	int i;
	int j;
	int ret;
	int is_def_command;
	int number_command;

	i = 0;
	ret = pre_pars(term, &command_pipe);
	while (command_pipe[i] != NULL)
	{
		command_cur = command_pipe[i];
		if (ret && *command_cur != NULL)
		{
			number_command = check_not_def_com(*command_cur, term->not_def_command);// возможно эти команды надо делать отдельным процессом, но хз
			if (number_command == -1)												// проверка команд (они не дефолтные?)
				is_def_command = check_def_command(&command_cur, term);				// Они не дефолтные! И есть в папке /bin. Или это не команды.
		}
		i++;
	}
	i = 0;
	while (command_pipe[i] != NULL)
	{
		j = 0;
		command_cur = command_pipe[i];
		if (ret && *command_cur != NULL && (is_def_command || number_command != -1))
		{
			if (number_command != -1)
				pars_not_def_command(&command_cur, term, number_command);	// обработка не дефолтных команд
			else
				pars_def_command(&command_cur, term);						// обработка дефолтных команд
		}
		while (command_cur[j] != NULL)
		{
			free(command_cur[j]);
			j++;
		}
		free(command_cur);
		i++;
	}
	free(command_pipe);
}

void init_term_fd(t_terminal *term) //инициализация потоков
{
	term->fd.in = STDIN;
	term->fd.out = STDOUT;
	term->fd.error = STDERROR;
}

void teminal(t_terminal *term) //чтение строк терминала
{
	init_term_fd(term); //переинициализация потоков
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
		tmp->name = ft_strndup(envp[i], ft_strclen(envp[i], '='));
		tmp->line = ft_strdup(envp[i] + ft_strclen(envp[i], '=') + 1);
		if (*env == NULL)
			tmp->next = NULL;
		else
			tmp->next = *env;
		*env = tmp;
		i--;
	}
}

void init_t_teminal(t_terminal *term, int argc, char **argv, char **envp)
{
	init_env(&term->env, envp);
	(void)argc;
	(void)argv;
	term->fd_history = -1;
	term->flag_export = 0;
	term->line = NULL;
	term->history_cmd = NULL;
	term->not_def_command[0] = "cd";
	term->not_def_command[1] = "export";
	term->not_def_command[2] = "unset";
	term->not_def_command[3] = "env";
	read_file_history(term);
}

int main(int argc, char **argv, char **envp)
{
	t_terminal term;

	signal(SIGTSTP, SIG_IGN);
	signal(SIGINT, ft_print_n);
	init_t_teminal(&term, argc, argv, envp);
	if (term.fd_history != -1)
		close(term.fd_history);
	term.fd_history = -1;
	while (1)
		teminal(&term);
	close(term.fd_history);
	return (0);
}

//-----------ЗАДАЧИ-----------
//	Реализовать перенаправление вывода. Перенаправление стандартных потоков терминала(возможно не надо).
/*{ https://www.opennet.ru/docs/RUS/bash_scripting_guide/c11620.html
	COMMAND_OUTPUT >
		# Перенаправление stdout (вывода) в файл.
		# Если файл отсутствовал, то он создется, иначе -- перезаписывается.
	: > filename
		# Операция > усекает файл "filename" до нулевой длины.
		# Если до выполнения операции файла не существовало,
		# то создается новый файл с нулевой длиной (тот же эффект дает команда 'touch').
		# Символ : выступает здесь в роли местозаполнителя, не выводя ничего.

	> filename
		# Операция > усекает файл "filename" до нулевой длины.
		# Если до выполнения операции файла не существовало,
		# то создается новый файл с нулевой длиной (тот же эффект дает команда 'touch').
		# (тот же результат, что и выше -- ": >", но этот вариант неработоспособен
		# в некоторых командных оболочках.)

	COMMAND_OUTPUT >>
		# Перенаправление stdout (вывода) в файл.
		# Создает новый файл, если он отсутствовал, иначе -- дописывает в конец файла.
		# Однострочные команды перенаправления
		# (затрагивают только ту строку, в которой они встречаются):
		# --------------------------------------------------------------------

	1>filename
		# Перенаправление вывода (stdout) в файл "filename".
	1>>filename
		# Перенаправление вывода (stdout) в файл "filename", файл открывается в режиме добавления.
	2>filename
		# Перенаправление stderr в файл "filename".
	2>>filename
		# Перенаправление stderr в файл "filename", файл открывается в режиме добавления.
	&>filename
		# Перенаправление stdout и stderr в файл "filename".

		#==============================================================================
		# Перенаправление stdout, только для одной строки.
		LOGFILE=script.log

		echo "Эта строка будет записана в файл \"$LOGFILE\"." 1>$LOGFILE
		echo "Эта строка будет добавлена в конец файла \"$LOGFILE\"." 1>>$LOGFILE
		echo "Эта строка тоже будет добавлена в конец файла \"$LOGFILE\"." 1>>$LOGFILE
		echo "Эта строка будет выведена на экран и не попадет в файл \"$LOGFILE\"."
		# После каждой строки, сделанное перенаправление автоматически "сбрасывается".



		# Перенаправление stderr, только для одной строки.
		ERRORFILE=script.errors

		bad_command1 2>$ERRORFILE		#  Сообщение об ошибке запишется в $ERRORFILE.
		bad_command2 2>>$ERRORFILE		#  Сообщение об ошибке добавится в конец $ERRORFILE.
		bad_command3					#  Сообщение об ошибке будет выведено на stderr,
										#+ и не попадет в $ERRORFILE.
		# После каждой строки, сделанное перенаправление также автоматически "сбрасывается".
		#==============================================================================


	2>&1
		# Перенаправляется stderr на stdout.
		# Сообщения об ошибках передаются туда же, куда и стандартный вывод.

	i>&j
		# Перенаправляется файл с дескриптором i в j.
		# Вывод в файл с дескриптором i передается в файл с дескриптором j.

	>&j
		# Перенаправляется  файл с дескриптором 1 (stdout) в файл с дескриптором j.
		# Вывод на stdout передается в файл с дескриптором j.

	0< FILENAME
	< FILENAME
		# Ввод из файла.
		# Парная команде ">", часто встречается в комбинации с ней.
		# grep search-word <filename


	[j]<>filename
		# Файл "filename" открывается на чтение и запись, и связывается с дескриптором "j".
		# Если "filename" отсутствует, то он создается.
		# Если дескриптор "j" не указан, то, по-умолчанию, бередся дескриптор 0, stdin.
		#
		# Как одно из применений этого -- запись в конкретную позицию в файле.
		echo 1234567890 > File		# Записать строку в файл "File".
		exec 3<> File				# Открыть "File" и связать с дескриптором 3.
		read -n 4 <&3				# Прочитать 4 символа.
		echo -n . >&3				# Записать символ точки.
		exec 3>&-					# Закрыть дескриптор 3.
		cat File					# ==> 1234.67890
		# Произвольный доступ, да и только!
}*/

//	Сделать пайпы.
//	ЕСТЬ ЕБУЧИЙ ЛИК, КОТОРЫЙ Я НЕ МОГУ ПОФИКСТИЬ !!!!!!!!!!!!!!!!!!!!!!!!!!!

//-----------НЕ ЗАБЫТЬ-----------
//	dup2(fd, 1); перенаправлени стандартного вывода в fd

//-----------ЗАМЕЧЕННЫЕ БАГИ-----------
// unset не всегда удаляет переменную
// env с аргументом должен выводить ошибку
// Бывает, что нет перевода строки терминала при нажатии всяких кнопок с ctrl

//-----------ТЕСТИРОВАТЬ-----------
//	Тестировать кавычки и переменные среды
//	env LA=
//	export $ABC = sad
//	exit rasd
//	exit s
//	exitd
//	a=linuxcareer.com; echo $a linuxcareer.com
