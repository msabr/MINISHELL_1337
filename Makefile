# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msabr <msabr@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/07 02:38:10 by msabr             #+#    #+#              #
#    Updated: 2025/06/27 19:01:09 by msabr            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

RM = rm -f

CC = cc

CFLAGS = -Wall -Wextra -Werror  #-fsanitize=address,undefined 

LIBFT = Libft/libft.a

SRC_LIB =	Libft/ft_atoi.c Libft/ft_bzero.c Libft/ft_calloc.c Libft/ft_isalnum.c Libft/ft_isalpha.c \
			Libft/ft_isascii.c Libft/ft_isdigit.c Libft/ft_isprint.c Libft/ft_isspace.c Libft/ft_is_number.c \
			Libft/ft_memcmp.c Libft/ft_memcpy.c Libft/ft_memmove.c Libft/ft_memset.c Libft/ft_strchr.c \
			Libft/ft_strdup.c Libft/ft_strlcat.c Libft/ft_strlcpy.c Libft/ft_strlen.c Libft/ft_strncmp.c \
			libft/ft_strstr.c Libft/ft_strnstr.c Libft/ft_strrchr.c Libft/ft_tolower.c Libft/ft_toupper.c Libft/ft_memchr.c \
			Libft/ft_itoa.c Libft/ft_putchar_fd.c Libft/ft_putendl_fd.c Libft/ft_putnbr_fd.c Libft/ft_putstr_fd.c \
			Libft/ft_strjoin.c Libft/ft_split.c Libft/ft_strmapi.c Libft/ft_strtrim.c Libft/ft_substr.c Libft/ft_striteri.c\
			Libft/ft_split_space.c Libft/ft_strcmp.c Libft/ft_strcpy.c Libft/ft_strrev.c

SRC_Builtins =	execution/builtins/cd.c execution/builtins/echo.c execution/builtins/env.c \
				execution/builtins/export.c execution/builtins/pwd.c execution/builtins/unset.c \
				execution/builtins/exit.c execution/builtins/external_files.c\
				execution/builtins/internal_files.c execution/builtins/export_files.c

SRC_ENV =	execution/environment/file1.c execution/environment/file2.c 


SRC_REDIRECT =	execution/redirection/heredoc.c execution/redirection/redirect_append.c execution/redirection/redirect_overwrite.c\
				execution/redirection/redirect_stdin.c execution/redirection/redirection_files.c 
		
SRCS = 	$(SRC_Builtins) $(SRC_REDIRECT) $(SRC_ENV) execution/execve.c\
		main.c \

OBJS = $(SRCS:.c=.o)

HEADERS = Libft/libft.h minishell.h


all: $(NAME)

$(NAME): $(LIBFT) $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) -lreadline
	make clean

%.o: %.c $(HEADERS) $(SRC_LIB)
	$(CC) $(CFLAGS) -c $< -o $@

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

.PHONY: clean