# Makefile pour le projet minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRC = main.c memory.c token_utils.c utils.c lexer2.c dubaging.c syntax_error.c parser.c
OBJ = $(SRC:.c=.o)
NAME = minishell

READLINE_COMPILE = -I$(shell brew --prefix readline)/include
READLINE_LINK = -lreadline -L$(shell brew --prefix readline)/lib

all: $(NAME)

$(NAME): $(OBJ) ../minishell.h
	$(CC) $(READLINE_COMPILE) $(CFLAGS) -o $(NAME) $(OBJ) $(READLINE_LINK)

%.o: %.c minishell.h
	$(CC) $(READLINE_COMPILE) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

run: $(NAME)
	./$(NAME)

install-readline:
	brew install readline

.PHONY: all clean fclean re run install-readline