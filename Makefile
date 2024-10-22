# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mohilali <mohilali@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/05 08:03:16 by wbelfatm          #+#    #+#              #
#    Updated: 2024/10/21 11:34:12 by mohilali         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address -g
SRC = main.cpp Webserv.cpp parsing/Parser.cpp utils/ListNode.cpp utils/ServerNode.cpp \
	  utils/Field.cpp utils/Location.cpp utils/Client.cpp \
	  Request/Request.cpp  Request/PostMethode.cpp \
	  Response/Response.cpp Response/PostMethode.cpp Response/MimeType.cpp
OBJ = ${SRC:.cpp=.o}
NAME = webserv
HEADERS = Webserv.hpp parsing/Parser.hpp utils/ListNode.hpp utils/ServerNode.hpp \
	  utils/Field.hpp utils/Location.hpp utils/Client.hpp Request/Request.hpp Response/Response.hpp
CC = c++
RM = rm -f

all: $(NAME)

%.o: %.cpp $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@ -g

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -g

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re