/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhali <skhali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 13:16:44 by valentin          #+#    #+#             */
/*   Updated: 2023/07/19 15:01:28 by skhali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

#include <iostream>
#include <vector>
#include <list>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include <signal.h>

#define MAX_CLIENTS 20
#define SERVER_NAME "server-irc"
#define BUFFER_SIZE 1024
#define ANSI_RESET "\033[0m"
#define ANSI_GREEN "\033[32m"
#define ANSI_BLUE "\033[94m"
#define ANSI_YELLOW "\033[33m"


class User;
class Server;
class Channel;

void server_exec(Server &server);
std::string find_next_word(int i, std::string str);
std::string find_previous_word(int i, std::string str);
void parse_buffer(std::string buffer, Server &server, int fd);
void send_function(Server &server, std::vector<pollfd> fds);
std::string msg_001(std::string user);
void msg_464();
void msg_462();
void msg_421();
std::string msg_353(Server &server, std::string channel);
int is_port(char *str);
void join_funct(std::string buffer, Server &server, int fd);
void privmsg_funct(std::string buffer, Server &server, int fd);
void send_whitelist(Server &server, int fd, std::string channel, std::string buffer);


#endif