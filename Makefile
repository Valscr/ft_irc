# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: smecili <smecili@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/07 20:51:27 by valentin          #+#    #+#              #
#    Updated: 2023/07/20 17:59:54 by smecili          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#******************************************************************************#

SRCS_DIR = srcs
SRCS_FILES			= main.cpp\
					User.cpp\
					Commands.cpp\
					Channel.cpp\
					Server.cpp\
					irc.cpp\
					msg.cpp\
					server_exec.cpp\
					utils.cpp\
					command.cpp\
SRCS = $(addprefix $(SRCS_DIR), $(SRCS_FILES))

#******************************************************************************#

OBJS_DIR		= objs
OBJS			= $(SRCS:%.c=$(OBJS_DIR)/%.o)

#******************************************************************************#

CC				= c++ -std=c++98
RM				= rm -f
WFLAGS		    = -Wall -Wextra -Werror -I.

#******************************************************************************#

NAME			= server
%.o: %.cpp
	$(CC) $(WFLAGS) -c $< -o $@
all:			$(NAME)

$(NAME):		$(OBJS_DIR) $(OBJS)
				$(CC) $(WFLAGS) -o $(NAME) $(OBJS)
$(OBJS_DIR) :
	mkdir $(OBJS_DIR)
clean:
				rm -rf $(OBJS_DIR)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY:			all clean fclean re