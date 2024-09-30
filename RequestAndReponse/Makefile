# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/24 12:43:46 by mohilali          #+#    #+#              #
#    Updated: 2024/09/30 13:25:49 by mohilali         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS = -Wall -Wextra -Werror #-std=c++98
SRC = request.cpp requestparsing.cpp GetMethode.cpp Response.cpp 
OBJ = ${SRC:.cpp=.o}
NAME = webserv
HEADERS = request.hpp
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