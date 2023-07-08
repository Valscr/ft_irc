/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:49:25 by valentin          #+#    #+#             */
/*   Updated: 2023/07/08 20:00:53 by valentin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "irc.hpp"

Server::Server(int port, std::string password): fds(MAX_CLIENTS + 1), send_client(0)
{
    this->port = port;
    this->password = password;
    this->name = SERVER_NAME;
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

std::string Server::get_name()
{
    return (this->name);
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

std::vector<std::string>  &Server::get_send()
{
    return (this->send_client);
}

void Server::createUser(std::string nickname, std::string username, int i)
{
    if (!nickname.empty())
    {
        for (int f = 1; f > 0;)
        {
            f = 0;
            for (int j = 0; static_cast<std::vector<bool>::size_type>(j) < this->users.size(); j++)
            {
                if (nickname == this->users[j].returnNickname() && j != i)
                {  
                    nickname = (nickname + "_").c_str();
                    f++;
                }
            }
        }     
    }
    User newUser(nickname, username, i);
    this->users.push_back(newUser);
}

User& Server::getUser(int i)
{
    for (size_t j = 0; j <= this->users.size(); j++)
    {
        if (this->users[j].returnFd() == i)
        {
            return this->users[j];
        }
    }
    // Gérer le cas où l'utilisateur n'est pas trouvé
    throw std::runtime_error("Utilisateur non trouvé");
}

void Server::deleteUser(int i)
{
     for (size_t j = 0; j <= this->users.size(); j++)
    {
        if (this->users[j].returnFd() == i)
        {
            this->users.erase(this->users.begin() + j);
            return ;
        }
    }
    throw std::runtime_error("Utilisateur non trouvé");
}
void Server::createChannel(std::string name, int fd)
{
        Channel newChannel(name, fd);
        this->channels.push_back(newChannel);
}

Channel& Server::getChannel(std::string name)
{
    for (size_t j = 0; j <= this->channels.size(); j++)
    {
        if (this->channels[j].getName() == name)
        {
            return this->channels[j];
        }
    }
    // Gérer le cas où l'utilisateur n'est pas trouvé
    throw std::runtime_error("Channel non trouvé");
}