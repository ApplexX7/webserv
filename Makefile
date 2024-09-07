# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wbelfatm <wbelfatm@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/05 08:03:16 by wbelfatm          #+#    #+#              #
#    Updated: 2024/09/07 14:43:54 by wbelfatm         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS = -Wall -Wextra -Werror -std=c++98
SRC = main.cpp parsing/Parser.cpp utils/ListNode.cpp
OBJ = ${SRC:.cpp=.o}
NAME = webserv
HEADERS = webserv.hpp parsing/Parser.hpp utils/ListNode.hpp
CC = c++
RM = rm -f

all: $(NAME)

%.o: %.cpp $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re