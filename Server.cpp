/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:49:25 by valentin          #+#    #+#             */
/*   Updated: 2023/07/07 23:00:34 by valentin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "irc.hpp"

Server::Server(int port, std::string password): fds(MAX_CLIENTS + 1)
{
    this->port = port;
    this->password = password;
    this->listenSocket = (socket(AF_INET, SOCK_STREAM, 0));
    if (this->listenSocket < 0)
        throw std::runtime_error("Error: creation socket.\n");
    memset(&this->serverAddress, 0, sizeof(this->serverAddress));
    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_addr.s_addr = INADDR_ANY;
    this->serverAddress.sin_port = htons(port);
    if (bind(this->listenSocket, reinterpret_cast<struct sockaddr*>(&this->serverAddress), sizeof(this->serverAddress)) < 0)
        throw std::runtime_error("Error: creation socket.\n");
    if (listen(this->listenSocket, MAX_CLIENTS) < 0)
        throw std::runtime_error("Error: creation socket.\n");
    if (fcntl(this->listenSocket, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("Error: creation socket.\n");
    for (int j = 0; j <= MAX_CLIENTS; j++)
    {
        this->fds[j].fd = this->listenSocket;
        this->fds[j].events = POLLIN | POLLOUT;
    }
}

void Server::close_fd(int i)
{
    close(this->fds[i].fd);
    this->fds[i].fd = -1;
}

void Server::set_fds_i_fd(int i)
{
    this->fds[i].fd = this->clientSocket;
    this->fds[i].events = POLLIN;
}

std::string Server::get_password()
{
    return (this->password);
}

void Server::set_Clientsocket(int clientsocket)
{
    this->clientSocket = clientsocket;
}

int Server::getListensocket()
{
    return (this->listenSocket);
}

int Server::getClientsocket()
{
    return (this->clientSocket);
}

std::vector<pollfd> &Server::get_fds()
{
    return (this->fds);
}

/*void Server::createChannel()
{
        Channel newChannel();
        this->channels.push_back(newChannel);
}

void Server::createUser()
{
        Channel newUser();
        this->users.push_back(newUser);
}*/