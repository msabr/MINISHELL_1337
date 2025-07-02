# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msabr <msabr@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/19 16:26:22 by msabr             #+#    #+#              #
#    Updated: 2025/07/01 17:46:09 by msabr            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CFLAGS = -Wall -Wextra -Werror

CC = cc

SRC = ft_atoi.c ft_itoa.c ft_bzero.c ft_calloc.c ft_malloc.c ft_free.c\
		ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_isspace.c ft_is_number.c\
		ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c\
		ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c\
		ft_strjoin.c  ft_strmapi.c ft_strtrim.c ft_substr.c ft_striteri.c\
		ft_strrev.c ft_strlen.c ft_tolower.c ft_toupper.c\
		ft_split.c ft_split_space.c\
		ft_strndup.c ft_strdup.c\
		ft_strrchr.c ft_strchr.c\
		ft_strnstr.c ft_strstr.c\
		ft_strlcat.c ft_strcat.c\
		ft_strlcpy.c ft_strcpy.c\
		ft_strncmp.c ft_strcmp.c 
		 
		 
		
		

OBJS =  ${SRC:.c=.o}

HEADER = libft.h

all: $(NAME)

$(NAME): $(OBJS)
	ar -rc $(NAME) $(OBJS)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all