# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: valentin <valentin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/07 20:51:27 by valentin          #+#    #+#              #
#    Updated: 2023/07/09 22:55:56 by valentin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS			= main.cpp User.cpp Channel.cpp Server.cpp irc.cpp msg.cpp \
					server_exec.cpp utils.cpp command.cpp
OBJS			= $(SRCS:.cpp=.o)

CC				= c++ -std=c++98
RM				= rm -f
WFLAGS		= -Wall -Wextra -Werror -I.

NAME			= server
%.o: %.cpp
	$(CC) $(WFLAGS) -c $<
all:			$(NAME)

$(NAME):		$(OBJS)
				$(CC) $(WFLAGS) -o $(NAME) $(OBJS)

clean:
				$(RM) $(OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY:			all clean fclean re