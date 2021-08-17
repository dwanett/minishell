/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gparsnip <gparsnip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 23:57:29 by dwanetta          #+#    #+#             */
/*   Updated: 2021/08/17 17:53:29 by gparsnip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	last_arg(t_terminal *term, t_info_command **command_cur, int *j)
{
	while ((*command_cur)->command != NULL
		&& (*command_cur)->command[*j] != NULL)
	{
		if ((*command_cur)->command[*j + 1] == NULL
			&& term->flag.export != 2 && term->flag.def_com == 0)
			update_variable_env(term, NULL, (*command_cur)->command[*j], NULL);
		if (*j == 0)
			term->flag.def_com = 0;
		free((*command_cur)->command[*j]);
		(*j)++;
	}
}

void	command(t_terminal *term)
{
	char			***command_pipe;
	t_info_command	*command_cur;
	t_info_command	*tmp;
	int				j;
	int				ret;

	ret = pre_pars(term, &command_pipe, &command_cur, -1);
	get_info_str_command(&command_cur, term, command_pipe, ret);
	while (command_cur != NULL)
	{
		j = 0;
		pars_command(term, &command_cur, ret);
		last_arg(term, &command_cur, &j);
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

//чтение строк терминала
void	teminal(t_terminal *term)
{
	if (init_term_fd(term) && term->line != NULL)
		free(term->line);
	term->line = readline("minishell$ ");
	if (term->line == NULL || !ft_strnccmp(term->line, "exit", ' ', 5))
		ft_exit(term);
	else if (ft_strcmp(term->line, "\0"))
	{
		if (term->history_cmd)
		{
			if (ft_strcmp(term->line, term->history_cmd->command))
			{
				ft_add_history(term);
				add_history(term->line);
			}
		}
		else
		{
			ft_add_history(term);
			add_history(term->line);
		}
		command(term);
		if (term->flag.error == 1)
			ft_putstr_fd(";: error syntax\n", term->fd.error);
		term->flag.error = 0;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_terminal	term;

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
//	Реализовать перенаправление вывода. Перенаправление стандартных потоков
//			терминала(возможно не надо).
//{ https://www.opennet.ru/docs/RUS/bash_scripting_guide/c11620.html
//	COMMAND_OUTPUT >
//		# Перенаправление stdout (вывода) в файл.
//		# Если файл отсутствовал, то он создется, иначе -- перезаписывается.
//	: > filename
//		# Операция > усекает файл "filename" до нулевой длины.
//		# Если до выполнения операции файла не существовало,
//		# то создается новый файл с нулевой длиной
//			(тот же эффект дает команда 'touch').
//		# Символ : выступает здесь в роли местозаполнителя, не выводя ничего.
//
//	> filename
//		# Операция > усекает файл "filename" до нулевой длины.
//		# Если до выполнения операции файла не существовало,
//		# то создается новый файл с нулевой длиной
//							(тот же эффект дает команда 'touch').
//		# (тот же результат, что и выше -- ": >",
//							но этот вариант неработоспособен
//		# в некоторых командных оболочках.)
//
//	COMMAND_OUTPUT >>
//		# Перенаправление stdout (вывода) в файл.
//		# Создает новый файл, если он отсутствовал,
//							иначе -- дописывает в конец файла.
//		# Однострочные команды перенаправления
//		# (затрагивают только ту строку, в которой они встречаются):
//		# --------------------------------------------------------------------
//
//	1>filename
//		# Перенаправление вывода (stdout) в файл "filename".
//	1>>filename
//		# Перенаправление вывода (stdout) в файл "filename",
//							файл открывается в режиме добавления.
//	2>filename
//		# Перенаправление stderr в файл "filename".
//	2>>filename
//		# Перенаправление stderr в файл "filename",
//							файл открывается в режиме добавления.
//	&>filename
//		# Перенаправление stdout и stderr в файл "filename".
//
//		#================================================================
//		# Перенаправление stdout, только для одной строки.
//		LOGFILE=script.log
//
//		echo "Эта строка будет записана в файл \"$LOGFILE\"." 1>$LOGFILE
//		echo "Эта строка будет добавлена в конец файла \"$LOGFILE\"." 1>>$LOGFILE
//		echo "Эта строка тоже будет добавлена в конец файла
//										\"$LOGFILE\"." 1>>$LOGFILE
//		echo "Эта строка будет выведена на экран и
//										не попадет в файл \"$LOGFILE\"."
//		# После каждой строки, сделанное
//								перенаправление автоматически "сбрасывается".
//
//
//
//		# Перенаправление stderr, только для одной строки.
//		ERRORFILE=script.errors
//
//		bad_command1 2>$ERRORFILE	# Сообщение об ошибке
//												запишется в $ERRORFILE.
//		bad_command2 2>>$ERRORFILE	# Сообщение об ошибке
//												добавится в конец $ERRORFILE.
//		bad_command3				# Сообщение об ошибке
//												будет выведено на stderr,
//										#+ и не попадет в $ERRORFILE.
//		# После каждой строки, сделанное перенаправление
//										также автоматически "сбрасывается".
//		#===============================================================
//
//
//	2>&1
//		# Перенаправляется stderr на stdout.
//		# Сообщения об ошибках передаются туда же, куда и стандартный вывод.
//
//	i>&j
//		# Перенаправляется файл с дескриптором i в j.
//		# Вывод в файл с дескриптором i передается в файл с дескриптором j.
//
//	>&j
//		# Перенаправляется файл с дескриптором
//								1 (stdout) в файл с дескриптором j.
//		# Вывод на stdout передается в файл с дескриптором j.
//
//	0< FILENAME
//	< FILENAME
//		# Ввод из файла.
//		# Парная команде ">", часто встречается в комбинации с ней.
//		# grep search-word <filename
//
//
//	[j]<>filename
//		# Файл "filename" открывается на
//							чтение и запись, и связывается с дескриптором "j".
//		# Если "filename" отсутствует, то он создается.
//		# Если дескриптор "j" не указан, то, //
//								по-умолчанию, бередся дескриптор 0, stdin.
//		#
//		# Как одно из применений этого -- запись в конкретную позицию в файле.
//		echo 1234567890 > File		# Записать строку в файл "File".
//		exec 3<> File				# Открыть "File" и связать с дескриптором 3.
//		read -n 4 <&3				# Прочитать 4 символа.
//		echo -n . >&3				# Записать символ точки.
//		exec 3>&-					# Закрыть дескриптор 3.
//		cat File					# ==> 1234.67890
//		# Произвольный доступ, да и только!
//}*/
//
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
