/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 13:16:44 by valentin          #+#    #+#             */
/*   Updated: 2023/07/08 20:49:34 by valentin         ###   ########.fr       */
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

#define MAX_CLIENTS 10
#define SERVER_NAME "server-irc"
#define BUFFER_SIZE 1024


class User;
class Server;
//class Channel;

std::string find_next_word(int i, std::string str);
void parse_buffer(std::string buffer, Server &server, int fd);
void send_function(std::vector<std::string> &send_client, int i, std::vector<pollfd> fds);
std::string msg_001(std::string user);
std::string msg_464();

#endif