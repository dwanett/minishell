/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 23:57:29 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/16 20:16:39 by gparsnip         ###   ########.fr       */
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

int	check_def_com(t_terminal *term, char *command, char **path) //Проверка /bin
{
	DIR				*dir;
	struct dirent	*dp;
	char			**patch_env;
	int				i;
	int				j;

	i = 0;
	j = 0;
	patch_env = NULL;
	if (term->path)
		patch_env = ft_split(term->path->line, ':'); // получение путей из переменной среды PATH
	while (patch_env && patch_env[i] != NULL)
	{
		dir = opendir(patch_env[i]);
		if (dir != NULL)// открываем каждую директорию и проверяем наличие команды
		{
			dp = readdir(dir);
			while (dp != NULL)
			{
				if (!ft_strcmp(command, dp->d_name))
				{
					closedir(dir);
					*path = ft_strdup(patch_env[i]);
					while (patch_env[j] != NULL)
					{
						free(patch_env[j]);
						j++;
					}
					free(patch_env);
					return (0);
				}
				dp = readdir(dir);
			}
			closedir(dir);
		}
		i++;
	}
	if (patch_env)
	{
		while (patch_env[j] != NULL)
		{
			free(patch_env[j]);
			j++;
		}
		free(patch_env);
	}
	return (1);
}

int	count_symbol_str(const char *str, char c)
{
	int	i;
	int	count;

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

int	is_path(const char *command)//Это путь?
{
	int	i;

	i = 0;
	while (command[i] != '\0')
	{
		if (command[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

int	check_def_command(char ***command, t_terminal *term)
{
	char *path;
	char *tmp;

	if (!is_path(**command)) // Если это не путь то
	{
		if (check_def_com(term, **command, &path)) //это команда? (если есть файл в /bin - это команда)
		{
			print_error(*command[0], "command not found", -1, term);
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
	update_variable_env(term, *command[0], ft_strrchr(*command[0], '/') + 1);
	term->flag.def_com = 1;
	pid = fork(); // создание потока для выполения команды
	if (pid == 0)
	{
		dup2(term->fd.in, 0);
		dup2(term->fd.out, 1);
		dup2(term->fd.error, 2);
		l = execve(*command[0], *command, term->start_env);
	}
	if (l == -1)
		print_error(*command[0], strerror(errno), 0, term);
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
		print_error(*(*command + 1), strerror(errno), 4, term);
		return ;
	}
}

void	pars_not_def_command(char ***command, t_terminal *term, int i) // Обработка не дефолтных
{
	char	**tmp;
	int		j;
	int		size_arg;

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
	update_variable_env(term, NULL, *command[0]);
	if (i == 0)
		ft_cd(command, j, term);
	else if (i == 1)
		ft_export(command, term, size_arg);
	else if (i == 2)
		ft_unset(command, term, size_arg);
	else if (i == 3)
		ft_env(term, 0, command);
}

int		tmp_variable(char ***command, t_terminal *term)
{
	t_list_env	*tmp;
	int			i;

	i = 0;
	while ((*command)[i] != NULL)
	{
		if (count_symbol_str((*command)[i], '=') == 0)
			return (1);
		i++;
	}
	i = 0;
	while ((*command)[i] != NULL)
	{
		tmp = (t_list_env *)malloc(sizeof(t_list_env));
		if (tmp == NULL)
			print_error(NULL, strerror(errno), 0, term);
		tmp->update_variable = NULL;
		tmp->name = ft_strndup((*command)[i], ft_strclen((*command)[i], '='));
		tmp->line = ft_strdup((*command)[i] + ft_strclen((*command)[i], '=') + 1);
		tmp->tmp_variable = 1;
		tmp->next = term->env;
		term->env = tmp;
		term->flag.export = 2;
		i++;
	}
	update_variable_env(term, NULL, "");
	return (0);
}

void	pipe_command(t_terminal *term, t_info_command **command_cur, char ***command_pipe)
{
	int	i;

	i = 0;
	while (command_pipe[i] != NULL)
		i++;
	if (i == 1)
	{
		(*command_cur)->fd.in = term->fd.in;
		(*command_cur)->fd.out = term->fd.out;
		(*command_cur)->fd.error = term->fd.error;
		(*command_cur)->fd.history = term->fd.history;
	}
	else
	{
		(*command_cur)->fd.in = -1;
		(*command_cur)->fd.out = -1;
		(*command_cur)->fd.error = term->fd.error;
	}
}

void get_info_str_command(t_info_command **command_cur, t_terminal *term, char ***command_pipe, int ret)
{
	t_info_command *last_elem;
	t_info_command *tmp;
	int				fd[2];
	int 			i;

	i = 0;
	fd[0] = -1;
	fd[1] = -1;
	*command_cur = NULL;
	while (command_pipe[i] != NULL)
	{
		tmp = (t_info_command*)malloc(sizeof(t_info_command));
		if (tmp == NULL)
			print_error(NULL, strerror(errno), 0, term);
		if (i == 0)
		{
			pipe_command(term, &tmp, command_pipe);
			if (tmp->fd.in != term->fd.in && tmp->fd.out != term->fd.out)
			{
				if (term->fd.out == STDOUT)
				{
					pipe(fd);
					tmp->fd.in = term->fd.in;
					tmp->fd.out = fd[1];
					tmp->fd.error = term->fd.error;
					tmp->fd.history = term->fd.history;
				}
				else
				{
				    pipe(fd);
					tmp->fd.in = term->fd.in;
					tmp->fd.out = term->fd.out;
					tmp->fd.error = term->fd.error;
					tmp->fd.history = term->fd.history;
				}
			}
		}
		tmp->is_def_command = 0;
		tmp->command = command_pipe[i];
		if (ret && *(tmp->command) != NULL)
		{
			tmp->number_command = check_not_def_com(*(tmp->command), term->not_def_command); // возможно эти команды надо делать отдельным процессом, но хз
			if (tmp->number_command == -1 && tmp_variable(&(tmp->command), term))												// проверка команд (они не дефолтные?)
				tmp->is_def_command = check_def_command(&(tmp->command), term);				// Они не дефолтные! И есть в папке /bin. Или это не команды.
		}
		tmp->next = NULL;
		if (i == 0)
		{
			(*command_cur) = tmp;
			last_elem = (*command_cur);
		}
		else
		{
			tmp->fd.in = fd[0];
			if (i == 1 && term->fd.out != STDOUT)
			{
			    write(fd[1], "\0", 1);
			    close(fd[1]);
			}
			if (command_pipe[i + 1] == NULL)
                tmp->fd.out = STDOUT;
			else
			{
				pipe(fd);
				//tmp->fd.in = fd[1];
				tmp->fd.out = fd[1];
			}
			tmp->fd.error = term->fd.error;
			tmp->fd.history = term->fd.history;
			last_elem->next = tmp;
			last_elem = last_elem->next;

		}
		i++;
	}
}

void	command(t_terminal *term)
{
	char			***command_pipe;
	t_info_command	*command_cur;
	t_info_command	*tmp;
	int				j;
	int				ret;

	ret = pre_pars(term, &command_pipe);
	get_info_str_command(&command_cur, term, command_pipe, ret);
	while (command_cur != NULL)
	{
		j = 0;
		term->fd = command_cur->fd;
		if (ret && *(command_cur->command) != NULL && (command_cur->is_def_command || command_cur->number_command != -1))
		{
			if (command_cur->number_command != -1)
				pars_not_def_command(&(command_cur->command), term, command_cur->number_command);	// обработка не дефолтных команд
			else
				pars_def_command(&(command_cur->command), term);						// обработка дефолтных команд
		}
		while (command_cur->command[j] != NULL)
		{
			if (command_cur->command[j + 1] == NULL && term->flag.export != 2 && term->flag.def_com == 0)
				update_variable_env(term, NULL, command_cur->command[j]);
			if (j == 0)
				term->flag.def_com = 0;
			free(command_cur->command[j]);
			j++;
		}
		if (term->fd.out != STDOUT)
			close(term->fd.out);
		if (term->fd.in != STDIN)
			close(term->fd.in);
		free(command_cur->command);
		tmp = command_cur;
		command_cur = command_cur->next;
		free(tmp);
	}
	free(command_pipe);
}

void	init_term_fd(t_terminal *term) //инициализация потоков
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
}

void	teminal(t_terminal *term) //чтение строк терминала
{
	init_term_fd(term); //переинициализация потоков
	if (term->line != NULL)
		free(term->line);
	term->line = readline("minishell$ ");
	if (term->line == NULL || !ft_strnccmp(term->line, "exit", ' ', 5)) // НАДО ПЕРЕНЕСТИ В КОМАНДЫ И ПРОВЕРИТЬ CASE exitr || exit r
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
		if (term->flag.error == 1)
			ft_putstr_fd(";: error syntax\n", term->fd.error);
		term->flag.error = 0;
	}
}

void	init_env(t_list_env **env, char **envp, t_terminal *term)
{
	t_list_env	*tmp;
	int			i;

	i = 0;
	while (envp[i] != NULL)
		i++;
	*env = NULL;
	i--;
	while(i >= 0)
	{
		tmp = (t_list_env*)malloc(sizeof(t_list_env));
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
		i--;
	}
}

void init_env_for_next_process(t_terminal *term, char **envp)
{
	int size_env;
	char *tmp;
	char *itoa;
	int j;

	size_env = 0;
	j = 0;
	while (envp[size_env] != NULL)
		size_env++;
	term->start_env = (char **)malloc(sizeof(char*) * (size_env + 1));
	if (term->start_env == NULL)
		print_error(NULL, strerror(errno), 0, term);
	while (j != size_env)
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
		j++;
	}
	term->start_env[j] = NULL;
}

void init_t_teminal(t_terminal *term, int argc, char **argv, char **envp)
{
	term->update = NULL;
	init_env_for_next_process(term, envp);
	init_env(&term->env, term->start_env, term);
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

int main(int argc, char **argv, char **envp)
{
	t_terminal term;

	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, ft_print_n);
	init_t_teminal(&term, argc, argv, envp);
	if (term.fd.history != -1)
		close(term.fd.history);
	term.fd.history = -1;
	while (1)
		teminal(&term);
	close(term.fd.history);
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
// не дефолтные команды не принимают через пайп

//-----------ЗАМЕЧЕННЫЕ БАГИ-----------
// unset не всегда удаляет переменную
// env с аргументом должен выводить ошибку
// Бывает, что нет перевода строки терминала при нажатии всяких кнопок с ctrl

//-----------ТЕСТИРОВАТЬ-----------
//	test6 <  grep HOME
// < test6 grep HOME
//	env > lol | grep HOME
//	Тестировать кавычки и переменные среды
//	a=linuxcareer.com; echo $a linuxcareer.com
//	exit rasd
//	exit s
//	exitd
