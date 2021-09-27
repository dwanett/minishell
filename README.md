# Описание
minishell - это проект школы 21, который требует реализации собственной оболочки. Она должна работать подобно bash <br/>

`pipes`, `semicolons`, `escape` `sequences`, `enviroment` `variables`, `$?`, `~`, перенаправление ввода/вывода: `>` `>>` `<`

Реализованные команды: `cd`, `pwd`, `echo`, `export`, `unset`, `env`, `exit`.

Сигналы: `Ctrl-C`, `Ctrl-\`, `Ctrl-D`.

Кавычки работают как в bash.<br/>
Команды работают с абсолютными и относительными путями.
# Компиляция
В корне репозитория
```
make
```
Исполняемый файл `minishell`

# Пример работы
![](https://github.com/dwanett/minishell/blob/master/gif/1.gif)
