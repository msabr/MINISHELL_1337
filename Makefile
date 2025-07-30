NAME = minishell

RM = rm -f

CC = cc

CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address,undefined 

LIBFT = Libft/libft.a

READLINE_PATH = $(shell brew --prefix readline)
READLINE_COMPILE = -I$(READLINE_PATH)/include
READLINE_LINK = -L$(READLINE_PATH)/lib -lreadline -lhistory -ltermcap

SRC_LIB= Libft/ft_atoi.c Libft/ft_itoa.c Libft/ft_bzero.c Libft/ft_calloc.c Libft/ft_malloc.c Libft/ft_free.c \
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

SRC_LEXER =	parsing/lexing/lexer.c \
			parsing/lexing/lexer_handlers.c \
			parsing/lexing/lexer_handler2.c \
			parsing/lexing/lexer_utils.c \
			parsing/lexing/token_utils.c

SRC_EXPANSION =	parsing/expension2/expend_helper.c \
				parsing/expension2/expend_helper2.c \
				parsing/expension2/expend_helper3.c \
				parsing/expension2/expend_helper4.c \
				parsing/expension2/expension.c \
				parsing/expension2/expend_utils.c \
				parsing/expension2/expend_utils2.c \
				parsing/expension2/expend_utils3.c \
				parsing/expension2/expend_utils4.c \
				parsing/expension2/expend_utils5.c \
				parsing/expension2/expend_herdoc.c

SRC_PARSE_CMD =	parsing/parse_cmd/parser.c \
				parsing/parse_cmd/parser_helper.c \
				parsing/parse_cmd/parse_utils.c \
				parsing/parse_cmd/parse_utils2.c \
				parsing/parse_cmd/parse_utils3.c \
				parsing/parse_cmd/export_parse.c

SRC_PAR =	$(SRC_LEXER) \
			parsing/check_syntax/syntax_error.c \
			parsing/check_syntax/syntax_tool.c \
			$(SRC_EXPANSION) \
			$(SRC_PARSE_CMD)


SRC_Builtins =	execution/builtins/cd.c execution/builtins/echo.c execution/builtins/env.c \
				execution/builtins/export.c execution/builtins/pwd.c execution/builtins/unset.c \
				execution/builtins/exit.c execution/builtins/external_files.c\
				execution/builtins/internal_files.c execution/builtins/export_files.c

SRC_ENV =	execution/environment/file1.c execution/environment/file2.c\
			execution/environment/file3.c

SRC_PIPE =	execution/pipe/handel_pipe1.c execution/pipe/handel_pipe2.c \
			execution/pipe/handel_pipe3.c execution/pipe/handel_pipe4.c

SRC_REDIRECT =	execution/redirection/redirect_append.c execution/redirection/redirect_overwrite.c\
				execution/redirection/redirect_stdin.c execution/redirection/redirection_files.c \
				execution/redirection/redirect_heredoc.c execution/redirection/redirect_heredoc_1.c
		
SRC_EXE =	$(SRC_Builtins)\
			$(SRC_ENV)\
			$(SRC_PIPE)\
			$(SRC_REDIRECT)\
			execution/exit_status.c\
			execution/file_descriptor.c\
			execution/main_loop.c\
			execution/path_functions_1.c\
			execution/path_functions.c\
			execution/print_errors.c\
			execution/signals.c\
			execution/simple_cmd.c

SRCS = 	$(SRC_EXE)\
		$(SRC_PAR)\
		main.c
		

OBJS = $(SRCS:.c=.o)

HEADERS =	Libft/libft.h\
 			minishell.h\
			parsing/parsing.h \
			execution/execution.h\
			execution/pipe/pipe.h \
			execution/builtins/builtins.h\
			execution/redirection/redirection.h

all: check-readline $(NAME)

$(NAME): $(LIBFT) $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(READLINE_COMPILE) $(OBJS) $(LIBFT) -o $(NAME) $(READLINE_LINK)
	
%.o: %.c $(HEADERS) $(SRC_LIB)
	$(CC) $(CFLAGS) $(READLINE_COMPILE) -c $< -o $@

$(LIBFT): $(SRC_LIB)
	make -C Libft

clean:
	make clean -C Libft
	$(RM) $(OBJS)

fclean: clean
	make fclean -C Libft
	$(RM) $(NAME)

re: fclean all

run:
	clear
	./$(NAME)

install-readline:
	brew install readline

.PHONY: all clean fclean re run install-readline check-readline