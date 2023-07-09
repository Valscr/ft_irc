/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:47:43 by valentin          #+#    #+#             */
/*   Updated: 2023/07/09 13:10:08 by valentin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "irc.hpp"
#include <iostream>
#include <vector>
#include <map>
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

class User;
class Channel;

class Server
{
    private:
        std::vector<pollfd> fds;
        std::string name;
        int port;
        std::string password;
        std::vector<Channel> channels;
        std::vector<User> users;
        struct sockaddr_in serverAddress;
        int listenSocket, clientSocket;
        std::map<int, std::string> send_client;
    public:
        Server(int port, std::string password);
        ~Server() {};
        void createChannel(std::string name, int fd);
        Channel& getChannel(std::string name);
        int find_channel(std::string name);
        void createUser(std::string nickname, std::string username, int i);
        void get_listen_socket(int listensocket);
        std::vector<pollfd> &get_fds();
        int getListensocket();
        int getClientsocket();
        void set_Clientsocket(int clientsocket);
        std::string get_password();
        void set_fds_i_fd(int i);
        void close_fd(int i);
        void deleteUser(int index);
        User& getUser(int i);
        std::string  &get_send_fd(int fd);
        std::map<int, std::string> &get_send();
        void  erase_send(int fd);
        void  add_send(int fd, std::string str);
        std::string get_name();
        User& getUserwithNickname(std::string name);
        int UserExist(std::string name);
};

#endif