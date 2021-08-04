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

/*void del_symbol(char **command, char c)
{
	char **tmp_split;
	char *tmp_join;
	int i;

	if (*command != NULL)
	{
		tmp_split = ft_split(*command , c);
		free(*command);
	}
	tmp_join = ft_strjoin(tmp_split[0], tmp_split[1]);
	free(tmp_split[0]);
	free(tmp_split[1]);
	i = 2;
	while(tmp_split[i] != NULL)
	{
		*command = ft_strjoin(tmp_join, tmp_split[i]);
		free(tmp_join);
		free(tmp_split[i]);
		tmp_join = *command;
		i++;
	}
}*/

int ligic_cavichki(char *command)//Проверка правильности расстановки кавычек
{
	int ferst_pos;
	int last_pos;
	char c;
	int i;

	i = 0;
	while (command[i] != '\0')
	{
		ferst_pos = 0;
		last_pos = 0;
		if (command[i] == '\'')
		{
			c = '\'';
			ferst_pos = i;
		}
		if (command[i] == '"')
		{
			c = '"';
			ferst_pos = i;
		}
		if (ferst_pos != 0)
		{
			while (command[i] != '\0')
			{
				i++;
				if (command[i] == c)
				{
					last_pos = i;
					break ;
				}
			}
		}
		if (ferst_pos != 0 && last_pos == 0)
			return (1);
		i++;
	}
	return (0);
}

int count_symbol_str(char *str, char c)
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

int pars_cavichki(char ***command, t_terminal *term)
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
	if (ligic_cavichki((*command)[0])) //А ты точно не хуету ввел?
	{
		ft_putstr_fd((*command)[0], term->fd.error);
		ft_putstr_fd(": ", term->fd.error);
		ft_putstr_fd("error syntax", term->fd.error);
		ft_putstr_fd("\n", term->fd.error);
		exit(-1);
	}
	count_one = count_symbol_str((*command)[0], '\'');
	count_two = count_symbol_str((*command)[0], '"');
	if (count_one % 2 != 0)
		count_one--;
	if (count_two % 2 != 0)
		count_two--;
	size = ft_strlen((*command)[0]) - count_one - count_two;
	tmp = (char *)malloc(sizeof(char) * size + 1);
	flag = 1;
	count_one = 0;
	count_two = 0;
	while ((*(*command))[i] != '\0')
	{
		if ((*(*command))[i] == '"' && count_one == 0)
		{
			if (count_two != 1)
				count_two++;
			else
				count_two = 0;
			flag = 2;
		}
		if ((*(*command))[i] == '\'' && count_two == 0)
		{
			if (count_one != 1)
				count_one++;
			else
				count_one = 0;
			flag = 1;
		}
		if (!(flag == 1 && (*(*command))[i] == '\'') && !(flag == 2 && (*(*command))[i] == '"'))
		{
			tmp[j] = (*(*command))[i];
			j++;
		}
		i++;
	} // Копи кавычек завершено
	tmp[j] = '\0';
	free((*command)[0]);
	(*command)[0] = tmp;
	return (0);
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

void pars_def_command(char ***command, t_terminal *term) // Обработка команд /bin или не команд // Не работает с переменными среды
{
	char *tmp;
	pid_t pid;
	int l;
	int status;
	char *path;

	if (!is_path(**command)) // Если это не путь то
	{
		if (check_def_com(**command, &path)) //это команда? (если есть файл в /bin - это команда)
		{
			ft_putstr_fd(*command[0], term->fd.error);
			ft_putstr_fd(": command not found", term->fd.error);
			ft_putstr_fd("\n", term->fd.error);
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

char *serch_env(char *name, t_terminal *term, int *i)
{
	t_list_env *tmp;
	int j;

	tmp = term->env;
	j = 0;
	while (tmp != NULL)
	{
		*i = ft_strclen(tmp->line, '=');
		if (!ft_strncmp(name, tmp->line, *i))
		{
			return (tmp->line + *i + 1);
		}
		tmp = tmp->next;
	}
	while (ft_isalpha(name[j]))
		j++;
	*i = j;
	return ("");
}


void pars_env_elem(t_terminal *term, char ***command_cur)
{
	int i;
	char *tmp;
	char *tmp_2;
	int open; //открытие '
	int size_name;

	i = 0;
	open = 0;
	while ((*(*command_cur))[i] != '\0')
	{
		if ((*(*command_cur))[i] == '$' && open == 0 && ft_isalpha((*(*command_cur))[i + 1]))
		{
			tmp = ft_strndup(*(*command_cur), i);
			tmp_2 = *(*command_cur);
			*(*command_cur) = ft_strjoin(tmp, serch_env(*(*command_cur) + i + 1, term, &size_name));
			free(tmp);
			tmp = ft_strdup(tmp_2 + i + 1 + size_name);
			free(tmp_2);
			tmp_2 = *(*command_cur);
			*(*command_cur) = ft_strjoin(tmp_2, tmp);
			free(tmp);
			free(tmp_2);
		}
		if ((*(*command_cur))[i] == '\'' && open == 1)
			open = 0;
		else if ((*(*command_cur))[i] == '\'')
			open = 1;
		i++;
	}
}

void pre_pars(t_terminal *term, char ****command_pipe)
{
	int size;
	int i;
	char **tmp;

	i = 0;
	tmp = ft_split(term->line, '|');
	size = ft_size_matrix_and_trim(tmp, " ");
	pars_env_elem(term, &tmp); //ДолларЧееек
	pars_cavichki(&tmp, term); // Чавички надо?
	*command_pipe = (char ***)malloc(sizeof(char**) * (size + 1));
	while (i != size)
	{
		(*command_pipe)[i] = ft_split(tmp[i], ' ');
		i++;
	}
	(*command_pipe)[i] = NULL;
}

void command(t_terminal *term)
{
	char *not_def_com[4];
	char ***command_pipe;
	char **command_cur;
	int i;
	int j;
	int ret;

	i = 0;
	not_def_com[0] = "cd"; //возможно эти команды надо делать отдельным процессом, но хз
	not_def_com[1] = "export";
	not_def_com[2] = "unset";
	not_def_com[3] = "env";
	pre_pars(term, &command_pipe);
	while (command_pipe[i] != NULL)
	{
		j = 0;
		command_cur = command_pipe[i];
		ret = check_not_def_com(*command_cur, not_def_com);
		if (ret != -1)									// проверка команд (они не дефолтные?)
			pars_not_def_command(&command_cur, term, ret); // обработка не дефолтных команд
		else									// Они не дефолтные! И есть в папке /bin. Или это не команды.
			pars_def_command(&command_cur, term);
		while (command_cur[j] != NULL)
		{
			free(command_cur[j]);
			j++;
		}
		free(command_cur);
		i++;
	}
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
	//exit(0);
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

void init_t_teminal(t_terminal *term, int argc, char **argv, char **envp)
{
	init_env(&term->env, envp);
	(void)argc;
	(void)argv;
	term->fd_history = -1;
	term->line = NULL;
	term->history_cmd = NULL;
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
//	Добавить поддержку переменных среды для команд.
//	Проверить разные последовательности символов. Экранирование, кавычки(двойные, одинарные), и другие.
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
//	export $ABC
//	env LA=
//	export $ABC = sad
//	export
//	exit rasd
//	exit s
//	exitd
//	a=linuxcareer.com; echo $a linuxcareer.com
