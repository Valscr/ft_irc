/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:49:25 by valentin          #+#    #+#             */
/*   Updated: 2023/07/09 22:53:44 by valentin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "irc.hpp"

Server::Server(int port, std::string password): fds(MAX_CLIENTS + 1)
{
    this->port = port;
    this->password = password;
    this->name = SERVER_NAME;
    this->listenSocket = (socket(AF_INET, SOCK_STREAM, 0));
    if (this->listenSocket < 0)
        throw std::runtime_error("creation socket.\n");
    memset(&this->serverAddress, 0, sizeof(this->serverAddress));
    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_addr.s_addr = INADDR_ANY;
    this->serverAddress.sin_port = htons(port);
    if (bind(this->listenSocket, reinterpret_cast<struct sockaddr*>(&this->serverAddress), sizeof(this->serverAddress)) < 0)
        throw std::runtime_error("creation socket.\n");
    if (listen(this->listenSocket, MAX_CLIENTS) < 0)
        throw std::runtime_error("creation socket.\n");
    if (fcntl(this->listenSocket, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("creation socket.\n");
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

std::string  &Server::get_send_fd(int fd)
{
    for (std::map<int, std::string>::iterator it = this->send_client.begin(); it != this->send_client.end(); ++it)
    {
        if (it->first == fd)
        {
            return (it->second);
        }
    }
     throw std::runtime_error("Socket non trouvé");
}

std::map<int, std::string> &Server::get_send()
{
    return (this->send_client);
}

void  Server::erase_send(int fd)
{
    for (std::map<int, std::string>::iterator it = this->send_client.begin(); it != this->send_client.end(); ++it)
    {
        if (it->first == fd)
        {
            it->second.clear();
        }
    }
}

void  Server::add_send(int fd, std::string str)
{
    for (std::map<int, std::string>::iterator it = this->send_client.begin(); it != this->send_client.end(); ++it)
    {
        if (it->first == fd)
        {
            it->second = str;
        }
    }
     throw std::runtime_error("Socket non trouvé");
}

void Server::createUser(std::string nickname, int i)
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
    User newUser(nickname, i);
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
    throw std::runtime_error("Utilisateur non trouvé");
}

int Server::UserExist(std::string name)
{
    for (size_t j = 0; j <= this->users.size(); j++)
    {
        if (this->users[j].returnNickname() == name)
        {
            
            return (1);
        }
    }
    return (0);
}

int Server::UserExist_fd(int fd)
{
    for (size_t j = 0; j <= this->users.size(); j++)
    {
        if (this->users[j].returnFd() == fd)
        {
            
            return (1);
        }
    }
    return (0);
}

User& Server::getUserwithNickname(std::string name)
{
    for (size_t j = 0; j <= this->users.size(); j++)
    {
        if (this->users[j].returnNickname() == name)
        {
            return this->users[j];
        }
    }
    throw std::runtime_error("Utilisateur non trouvé");
}

void Server::deleteUser(int i)
{
    this->erase_send(i);
    for (std::vector<Channel>::iterator it = this->channels.begin(); it != this->channels.end(); ++it)
    {
        Channel& channel = *it;
        channel.removeOperator(i);
        channel.removeWhiteList(i);
    }
    for (std::vector<User>::iterator it = this->users.begin(); it != this->users.end(); ++it)
    {
        if (it->returnFd() == i)
        {
            this->users.erase(it);
            return;
        }
    }
    throw std::runtime_error("Utilisateur non trouvé");
}
void Server::createChannel(std::string name, int fd)
{
        Channel newChannel(name, fd);
        this->channels.push_back(newChannel);
}

int Server::find_channel(std::string name)
{
    if (this->channels.empty())
        return (0);
    for (size_t j = 0; j <= this->channels.size(); j++)
    {
        if (this->channels[j].getName() == name)
        {
            return (1);
        }
    }
    return (0);
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
    throw std::runtime_error("Channel non trouvé");
}