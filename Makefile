NAME = minishell
CC = gcc
FLAGS = -Wall -Wextra #-Werror
LIB = -L libft/ -L ~/.brew/opt/readline/lib/
HEADER = -I ~/.brew/opt/readline/include/readline/
OPTION_LIB = -lft -ltermcap -lreadline
LIBFT = make -C libft

SRC =	minishell.c\
		terminal_utils.c\
		terminal_history.c\
		env.c\
		pars.c\
		errors.c\
		check_command.c\
		init.c\
		get_info_str.c\
		def_command.c\
		pars_command.c\
		export.c\
		unset.c\
		utils_export.c\
		pars_multi_command.c\
		pars_quotes.c\
		pars_utils.c\
		help_pars_quotes.c\
		ft_split_pipe.c

SRCS = $(SRC)

OBJ = $(SRCS:.c=.o)

all: $(NAME)

libft:
	@$(LIBFT)

$(NAME): $(OBJ) libft
	@echo "\n"
	@echo "\033[0;32mCompiling minishell..."
	@$(CC) $(FLAGS) $(OBJ) $(HEADER) $(LIB) $(OPTION_LIB) -o $(NAME)
	@echo "\n\033[0mDone !"

%.o: %.c
	@printf "\033[0;33mGenerating minishell objects... %-33.33s\r" $@
	@${CC} -g ${FLAGS} -c $< -o $@

clean:
	@$(LIBFT) clean
	@echo "\nRemoving binaries..."
	@rm -f $(OBJ)
	@echo "\033[0m"

fclean:
	@$(LIBFT) fclean
	@echo "\033[0;31m\nDeleting objects..."
	@rm -f $(OBJ)
	@echo "\nDeleting executable..."
	@rm -f $(NAME)
	@echo "\nDeleting trash..."
	@rm -f *.out
	@rm -f .history
	@echo "\033[0m"

re: fclean all

.PHONY: clean fclean re libft
