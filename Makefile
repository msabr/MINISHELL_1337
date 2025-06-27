# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msabr <msabr@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/07 02:38:10 by msabr             #+#    #+#              #
#    Updated: 2025/06/25 18:02:23 by msabr            ###   ########.fr        #
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
			Libft/ft_lstsize.c Libft/ft_lstnew.c Libft/ft_lstlast.c Libft/ft_lstclear.c Libft/ft_lstadd_back.c \
			Libft/ft_split_space.c Libft/ft_strcmp.c Libft/ft_strcpy.c Libft/ft_strrev.c

SRC_ENV = environment/file3.c #environment/file2.c
				
SRC_REDIRECT = redirection/lless_than_sign.c redirection/greagreater_than_sign.c \
				redirection/double_greater_than_sign.c redirection/double_less_than_sign.c \


SRC_Builtins =	builtins/cd.c builtins/echo.c builtins/env.c \
				builtins/export.c builtins/pwd.c builtins/unset.c \
				builtins/exit.c builtins/external_files.c\
				builtins/internal_files.c builtins/export_files.c builtins/export_files1.c\

SRC_EXECUTE = execution/environment.c execve.c 
		
SRCS = 	$(SRC_Builtins) $(SRC_EXECUTE) $(SRC_REDIRECT) $(SRC_ENV) \
		main.c \

OBJS = $(SRCS:.c=.o)

HEADERS = Libft/libft.h minishell.h builtins/builtins.h 


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