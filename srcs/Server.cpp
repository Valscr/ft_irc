/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhali <skhali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:49:25 by valentin          #+#    #+#             */
/*   Updated: 2023/07/24 15:33:56 by skhali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/irc.hpp"
#include "../includes/Commands.hpp"

/***********************************************************************/
/*                   CONSTRUCTOR & DESTRUCTOR                         */
/*********************************************************************/

Server::Server(int port, std::string password)
{
    this->port = port;
    this->password = password;
    this->name = SERVER_NAME;
    this->commands = new Commands();
    
    this->listenSocket = (socket(AF_INET, SOCK_STREAM, 0));
    if (this->listenSocket < 0)
    {
        this->freeEverything();
        throw std::runtime_error("creation socket 1.\n");
    }
    memset(&this->serverAddress, 0, sizeof(this->serverAddress));
    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_addr.s_addr = INADDR_ANY;
    this->serverAddress.sin_port = htons(port);
    if (bind(this->listenSocket, reinterpret_cast<struct sockaddr*>(&this->serverAddress), sizeof(this->serverAddress)) == -1)
    {
        this->freeEverything();
        throw std::runtime_error("creation socket 2.\n");
    }
    if (listen(this->listenSocket, MAX_CLIENTS) < 0)
    {
        this->freeEverything();
        throw std::runtime_error("creation socket 3.\n");
    }
    if (fcntl(this->listenSocket, F_SETFL, O_NONBLOCK) < 0)
    {
        this->freeEverything();
        throw std::runtime_error("creation socket 4.\n");
    }
    this->fds.push_back(pollfd());
    this->fds[0].fd = this->listenSocket;
    this->fds[0].events = POLLIN | POLLOUT;
}

/***********************************************************************/
/*                          SETTERS                                   */
/*********************************************************************/

void Server::set_fds_i_fd()
{
    int i = this->fds.size();
    this->fds.push_back(pollfd());
    this->fds[i].fd = this->clientSocket;
    this->fds[i].events = POLLIN | POLLRDHUP;
}

void Server::set_Clientsocket(int clientsocket)
{
    this->clientSocket = clientsocket;
}
/***********************************************************************/
/*                          GETTERS                                   */
/*********************************************************************/

std::string Server::get_name()
{
    return (this->name);
}

std::string Server::get_password()
{
    return (this->password);
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

Commands *Server::getCommand()
{
    return (this->commands);
}
std::map<int, std::string> &Server::get_send()
{
    return (this->send_client);
}



void  Server::add_send(int fd, std::string str)
{
    try {
		this->send_client.at(fd);
	} catch (std::exception const &e) {
		this->send_client.insert(std::make_pair(fd, ""));}
    //peut etre pas ajouter mais remplacer plutot
    //on risque de lire 2 fois les memes données peut etre sinon
    this->send_client[fd] = str;
}

std::vector<User> Server::getUsersList()
{
    return (this->users);
}

User& Server::getUser(int i)
{
    for (size_t j = 0; j < this->users.size(); j++)
    {
        if (this->users[j].returnFd() == i)
        {
            return this->users[j];
        }
    }
    throw std::runtime_error("getUser :Utilisateur non trouvé\n");
}

User& Server::getUserwithNickname(std::string name)
{
    for (size_t j = 0; j < this->users.size(); j++)
    {
        if (this->users[j].returnNickname() == name)
        {
            return this->users[j];
        }
    }
    throw std::runtime_error("getUserwithNickname :Utilisateur non trouvé");
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

/**************************************************************/
/*                 USER related functions                    */
/************************************************************/

void Server::createUser(std::string nickname, int id, int fd)
{
    if (!nickname.empty())
    {
        for (int f = 1; f > 0;)
        {
            f = 0;
            for (int j = 0; static_cast<std::vector<bool>::size_type>(j) < this->users.size(); j++)
            {
                if (nickname == this->users[j].returnNickname() && j != fd)
                {  
                    nickname = (nickname + "_").c_str();
                    f++;
                }
            }
        }     
    }
    User newUser(nickname, fd, id);
    this->users.push_back(newUser);
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

void Server::deleteUser(int i)
{
    this->erase_send(i);
    /*for (std::vector<Channel>::iterator it = this->channels.begin(); it != this->channels.end(); ++it)
    {
        Channel& channel = *it;
        if (channel.find_channels(i))
            send_whitelist(*this, i, channel.getName(), (": PRIVMSG #" + channel.getName() + " :" + this->getUser(i).returnNickname() + " has disconnected\r\n").c_str());
        channel.removeOperator(i);
        channel.removeWhiteList(i);
    }*/
    for (std::vector<User>::iterator it = this->users.begin(); it != this->users.end(); ++it)
    {
        if (it->returnFd() == i)
        {
            this->users.erase(it);
            return;
        }
    }
    throw std::runtime_error("deleteUser :Utilisateur non trouvé");
}

bool Server::NicknameMatching(std::string nickname)
{
    for (size_t j = 0; j < this->users.size(); j++)
    {
        if (this->users[j].returnNickname() == nickname)
        {
            return (true);
        }
    }
    return (false);
}
/**************************************************************/
/*              CHANNEL realted functions                    */
/************************************************************/


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

/**************************************************************/
/*                            Utils                          */
/************************************************************/

void Server::close_fd(int i)
{
    close(this->fds[i].fd);
    this->fds[i].fd = -1;
}

void Server::erase_fd(int i)
{
    int size = this->fds.size();
    if (i < size) {
        this->fds.erase(this->fds.begin() + i);
    } else {
        std::cout << "Position du fd invalide !" << std::endl;
    }
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

void Server::freeEverything()
{
    for (size_t i = 0; i < this->fds.size(); i++)
    {
        this->close_fd(i);
        this->erase_fd(i);
    }
    this->users.clear();
    this->send_client.clear();
    delete this->commands;
}