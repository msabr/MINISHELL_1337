NAME = minishell

RM = rm -f

CC = cc

CFLAGS = -Wall -Wextra -Werror  #-g -fsanitize=address,undefined

LIBFT = Libft/libft.a

READLINE_PATH = $(shell brew --prefix readline)
READLINE_COMPILE = -I$(READLINE_PATH)/include
READLINE_LINK = -L$(READLINE_PATH)/lib -lreadline -lhistory -ltermcap

SRC_LIB= Libft/ft_atoi.c Libft/ft_itoa.c Libft/ft_bzero.c Libft/ft_calloc.c Libft/ft_malloc.c Libft/ft_free.c\
		Libft/ft_isalnum.c Libft/ft_isalpha.c Libft/ft_isascii.c Libft/ft_isdigit.c Libft/ft_isprint.c Libft/ft_isspace.c Libft/ft_is_number.c\
		Libft/ft_memchr.c Libft/ft_memcmp.c Libft/ft_memcpy.c Libft/ft_memmove.c Libft/ft_memset.c\
		Libft/ft_putchar_fd.c Libft/ft_putendl_fd.c Libft/ft_putnbr_fd.c Libft/ft_putstr_fd.c\
		Libft/ft_strjoin.c Libft/ft_strmapi.c Libft/ft_strtrim.c Libft/ft_substr.c Libft/ft_striteri.c\
		Libft/ft_strrev.c Libft/ft_strlen.c Libft/ft_tolower.c Libft/ft_toupper.c\
		Libft/ft_split.c Libft/ft_split_space.c\
		Libft/ft_strndup.c Libft/ft_strdup.c\
		Libft/ft_strrchr.c Libft/ft_strchr.c\
		Libft/ft_strnstr.c Libft/ft_strstr.c\
		Libft/ft_strlcat.c Libft/ft_strcat.c\
		Libft/ft_strlcpy.c Libft/ft_strcpy.c\
		Libft/ft_strncmp.c Libft/ft_strcmp.c 

SRC_Builtins =	execution/builtins/cd.c execution/builtins/echo.c execution/builtins/env.c \
				execution/builtins/export.c execution/builtins/pwd.c execution/builtins/unset.c \
				execution/builtins/exit.c execution/builtins/external_files.c\
				execution/builtins/internal_files.c execution/builtins/export_files.c

SRC_ENV =	execution/environment/file1.c execution/environment/file2.c\
			execution/environment/file3.c

SRC_PIPE =	execution/pipe/handel_pipe1.c execution/pipe/handel_pipe2.c execution/pipe/handel_pipe3.c\

SRC_REDIRECT =	execution/redirection/heredoc.c execution/redirection/redirect_append.c execution/redirection/redirect_overwrite.c\
				execution/redirection/redirect_stdin.c execution/redirection/redirection_files.c 
		
SRC_PAR = parsing/dubaging.c parsing/expainding.c parsing/lexer2.c parsing/parser.c\
		parsing/syntax_error.c parsing/token_utils.c parsing/utils.c

SRCS = 	$(SRC_Builtins) $(SRC_REDIRECT) $(SRC_ENV) $(SRC_PAR) $(SRC_PIPE)\
		execution/path_functions.c main.c signals.c execution/simple_cmd.c

OBJS = $(SRCS:.c=.o)

HEADERS = Libft/libft.h minishell.h execution/pipe/pipe.h

all: check-readline $(NAME)

$(NAME): $(LIBFT) $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(READLINE_COMPILE) $(OBJS) $(LIBFT) -o $(NAME) $(READLINE_LINK)
	

%.o: %.c $(HEADERS) $(SRC_LIB)
	$(CC) $(CFLAGS) $(READLINE_COMPILE) -c $< -o $@

$(LIBFT): $(SRC_LIB)
	make -C Libft

exec:
	./$(NAME)

clean:
	$(RM) $(OBJS)
	make clean -C Libft

fclean: clean
	$(RM) $(NAME)
	make fclean -C Libft

re: fclean all

run: $(NAME)
	./$(NAME)

install-readline:
	brew install readline

.PHONY: all clean fclean re run install-readline check-readline
