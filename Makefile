# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vescaffr <vescaffr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/07 20:51:27 by valentin          #+#    #+#              #
#    Updated: 2023/08/30 16:13:12 by vescaffr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#******************************************************************************#

SRCS_DIR = srcs
SRCS_FILES = main.cpp \
             User.cpp \
             Channel.cpp \
             Server.cpp \
             irc.cpp \
             msg.cpp \
             server_exec.cpp \
             utils.cpp \
             Commands.cpp \
             Commands_bis.cpp \

SRCS = $(addprefix , $(SRCS_FILES))

#******************************************************************************#

OBJS_DIR = objs
OBJS = $(SRCS:%.cpp=$(OBJS_DIR)/%.o)

#******************************************************************************#

CC = c++ -std=c++98
RM = rm -f
WFLAGS = -Wall -Wextra -Werror -I.

#******************************************************************************#

NAME = ircserv
all: $(NAME)

$(NAME): $(OBJS_DIR) $(OBJS)
	$(CC) $(WFLAGS) -o $(NAME) $(OBJS)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OBJS): $(OBJS_DIR)/%.o : $(SRCS_DIR)/%.cpp
	$(CC) $(WFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re