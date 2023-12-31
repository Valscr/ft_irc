#include "../includes/Server.hpp"
#include "../includes/irc.hpp"
#include "../includes/Commands.hpp"
#include "../includes/Channel.hpp"

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
    this->fds[i].events = POLLIN | POLLHUP;
}

void Server::set_Clientsocket(int clientsocket)
{
    this->clientSocket = clientsocket;
}
/***********************************************************************/
/*                          GETTERS                                   */
/*********************************************************************/

int Server::find_fds(int i)
{
    int j = 0;
    for (std::vector<pollfd>::iterator it = this->fds.begin(); it != this->fds.end(); ++it)
    {
        if (it->fd == i)
            return (j);
        j++;
    }
    return (0);
}

pollfd Server::find_fds_fd(int i)
{
    int j = 0;
    for (std::vector<pollfd>::iterator it = this->fds.begin(); it != this->fds.end(); ++it)
    {
        if (it->fd == i)
            return (this->fds[j]);
        j++;
    }
    pollfd emptyPollfd = { -1, 0, 0 };  // Valeur par défaut pour indiquer "non trouvé"
    return (emptyPollfd);
}

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
    return (this->rcv_client);
}

std::map<int, std::string> &Server::get_send_client()
{
    return (this->send_client);
}

void  Server::addmsg_rcv(int fd, std::string str)
{
    try {
		this->rcv_client.at(fd);
	} catch (std::exception const &e) {
		this->rcv_client.insert(std::make_pair(fd, ""));}
    this->rcv_client[fd] = str;
}

void  Server::addmsg_send(int fd, std::string str)
{
    try {
		this->send_client.at(fd);
	} catch (std::exception const &e) {
		this->send_client.insert(std::make_pair(fd, ""));}
    this->send_client[fd].append(str);
}

std::vector<User> Server::getUsersList()
{
    return (this->users);
}

int Server::isUser_id(int id)
{
    for (size_t j = 0; j < this->users.size(); j++)
    {
        if (this->users[j].returnId() == id)
        {
            return (1);
        }
    }
    return (0);
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

Channel* Server::getChannel(std::string name)
{
    for (size_t j = 0; j < this->channels.size(); j++)
    {
        if ((*(this->channels[j])).getName() == name)
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
    User newUser(nickname, fd, id);
    this->users.push_back(newUser);
}

int Server::UserExist(std::string name)
{
    for (size_t j = 0; j < this->users.size(); j++)
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
    for (size_t j = 0; j < this->users.size(); j++)
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
    for (size_t j = 0; j < this->channels.size(); j++)
    {
        if ((*(this->channels[j])).find_channels(i))
        {
            (*(this->channels[j])).removeOperator(i);
            (*(this->channels[j])).removeWhiteList(i);
        }
    }
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

Channel *Server::createChannel(std::string name, int fd, std::string password, int id)
{
        Channel *newChannel= new Channel(name, fd, password, id);
        this->channels.push_back(newChannel);
        return (newChannel);
}

int Server::find_channel(std::string name)
{
    if (this->channels.empty())
        return (0);
    for (size_t j = 0; j < this->channels.size(); j++)
    {
        if ((*(this->channels[j])).getName() == name)
        {
            return (1);
        }
    }
    return (0);
}

void Server::send_all(std::string msg, Channel chan, int fd_client_actuel)
{
    (void)fd_client_actuel;
    for (std::vector<int>::iterator it = chan.getWhiteList().begin(); it != chan.getWhiteList().end(); ++it) {
        if (this->getListensocket() ==  *it)
            continue ;
        send(*it, msg.c_str(), msg.length(), 0);
    }
}
/**************************************************************/
/*                            Utils                          */
/************************************************************/


void Server::close_fd(int i)
{
    if (this->fds[i].fd > 0)
        close(this->fds[i].fd);
    this->fds[i].fd = -1;
}

void Server::erase_fd(int i)
{
    if (this->find_fds(i) != 0)
        this->fds.erase(this->fds.begin() + this->find_fds(i));
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
    for (std::vector<Channel*>::iterator it = this->channels.begin(); it != this->channels.end(); ++it) {
        delete *it;
    }
    this->channels.clear();
    this->rcv_client.clear();
    this->send_client.clear();
    delete this->commands;
}

User& Server::find_user(int id)
{
    for (size_t j = 0; j < this->users.size(); j++)
    {
        if (this->users[j].returnId() == id)
        {
            return this->users[j];
        }
    }
    throw std::runtime_error("find_user :Utilisateur non trouvé");

}