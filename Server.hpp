/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:47:43 by valentin          #+#    #+#             */
/*   Updated: 2023/07/08 12:17:40 by valentin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "irc.hpp"
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

class User;

class Server
{
    private:
        std::vector<pollfd> fds;
        std::string name;
        int port;
        std::string password;
        //std::vector<Channel> channels;
        std::vector<User> users;
        struct sockaddr_in serverAddress;
        int listenSocket, clientSocket;
        //std::vector<std::string> send_client;
    public:
        Server(int port, std::string password);
        ~Server() {};
        //void createChannel();
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
};

#endif