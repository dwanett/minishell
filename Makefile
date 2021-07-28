NAME = minishell
CC = gcc
FLAGS = -Wall -Wextra -Werror
LIB = -L libft/
OPTION_LIB = -lft
LIBFT = make -C libft

SRC = minishell.c

SRCS = $(SRC)

OBJ = $(SRCS:.c=.o)

all: $(NAME)

libft:
	@$(LIBFT)

$(NAME): $(OBJ) libft
	@echo "\n"
	@echo "\033[0;32mCompiling minishell..."
	@$(CC) $(FLAGS) $(OBJ) $(LIB) $(OPTION_LIB) -o $(NAME)
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
	@rm -f *.txt
	@rm -f *.out
	@echo "\033[0m"

re: fclean all

.PHONY: clean fclean re libft
