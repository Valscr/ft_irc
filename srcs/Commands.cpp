#include "../includes/Commands.hpp"
#include "../includes/Server.hpp"
#include "../includes/irc.hpp"
#include "../includes/msg.hpp"
#include "../includes/Channel.hpp"


/***********************************************************************/
/*                   CONSTRUCTOR & DESTRUCTOR                         */
/*********************************************************************/

typedef int (Commands::*fct)(std::vector<std::string> &, int, Server &);
Commands::Commands(void) 
{
    this->services_list = setServices();    
    return;
}
Commands::~Commands(void) { return; }


/***********************************************************************/
/*                         COMMANDS METHODS                           */
/*********************************************************************/

int Commands::PASS(std::vector<std::string> &command, int id, Server &server)
{
    int fd = server.get_fds()[id].fd;
    if (command.size() == 1)
    {
        msg_421();
        disconnect(id, server, false);
        return 0;
    }
    if (!server.getUser(fd).returnPassword() && (command[1] == server.get_password()))
    {
        server.getUser(fd).setPassword(true);
        return (1);
    }
    else if(server.getUser(fd).returnPassword())
    {
        msg_462();
        return (1);
    }
    else
    {
        msg_464();
        disconnect(id, server, false);
        return (0);
    }
}

int Commands::validNickname(std::string nickname)
{
    if((nickname[0] >= '0' && nickname[0] <= '9') || (nickname[0] ==  '-'))
        return (0);
    for (size_t i = 1; i < nickname.length(); i++) {
        if (((nickname[i] < '0') && (nickname[i] != '-')) || ((nickname[i] > '9') && (nickname[i] < 'A')) || (nickname[i] > '}'))
            return (0);
    }
    return (1);
}
int Commands::NICK(std::vector<std::string> &command, int id, Server &server)
{
    int fd = server.get_fds()[id].fd;
    if (command[1].empty())
    {
        msg_431();
        disconnect(id, server, false);
        return 0;
    }
    else if (!validNickname(command[1]))
    {
        msg_432();
        disconnect(id, server, false);
        return 0;
    }
    else if(server.NicknameMatching(command[1]))
    {
        msg_433();
        disconnect(id, server, false);
        return 0;
    }
    server.getUser(fd).setNickname(command[1]);
    server.getUser(fd).setHasNickname(true);
    send(fd, msg_001(server.getUser(fd).returnNickname()).c_str(), msg_001(server.getUser(fd).returnNickname()).length(), 0);
    std::cout << ANSI_GREEN << fd << " > " << msg_001(server.getUser(fd).returnNickname()).c_str() << ANSI_RESET << std::endl;
    return (1);
}

int Commands::USER(std::vector<std::string> &command, int id, Server &server)
{
    int fd = server.get_fds()[id].fd;

    if (command.size() < 5)
    {
        msg_421();
        disconnect(id, server, false);
        return 0;
    }
    else if(server.getUser(fd).returnRegistered())
    {
        msg_462();
        return 1;
    }
    if (server.UserExist_fd(fd) && server.getUser(fd).returnHasNickname())
    {
        server.getUser(fd).setUsername(command[1]);
        std::cout << "Username " << server.getUser(fd).returnUsername() << std::endl;
        server.getUser(fd).setRealsname(command[2]);
        std::cout << "Realsname " << server.getUser(fd).returnRealname() << std::endl;
        server.getUser(fd).setHotsname(command[4]);
        std::cout << "Hostname  " << server.getUser(fd).returnHostname() << std::endl;
        server.getUser(fd).setRegistered(true);
        int new_id = server.getUser(fd).returnId();
        std::cout << "Nickname  " << server.getUser(fd).returnNickname() << std::endl;
        std::cout << "Client " << new_id << "[" << fd << "]" << " connected" << std::endl;
        return (1);
    }
    else
    {
        std::cout << "User not found" << std::endl;
        return (0);
    }
}

int Commands::PING(std::vector<std::string> &command, int id, Server &server)
{
    int fd = server.get_fds()[id].fd;
    std::string msg = "PONG " + command[1] + "\r\n";
    send(fd, msg.c_str(), msg.length(), 0);
    std::cout << ANSI_GREEN << server.getUser(server.get_fds()[id].fd).returnId() << " > " << msg.c_str() << ANSI_RESET << std::endl;
    return (1);
}
/***********************************************************************/
/*                          GETTERS & SETTERS                         */
/*********************************************************************/

std::map<std::string, fct> Commands::getServices()
{
    return (this->services_list);
}

/***********************************************************************/
/*                          Sam's space                               */
/*********************************************************************/

int Commands::KICK(std::vector<std::string> &command, int id, Server &server)
{
    int id_client = server.getUser(server.get_fds()[id].fd).returnId();
    std::vector<int>::iterator vict;
    std::string reason;
    Channel it;
    int id_victim;
    bool found = false;

    if(command[1].empty())
    {
        send(server.get_fds()[id].fd, ERR_NOSUCHCHANNEL("Empty name").c_str(), ERR_NOSUCHCHANNEL("Empty name").length(), 0);
        return 1;
    }
    try
    {
       it = server.getChannel(command[1]);
    }
    catch(const std::exception& e)
    {
        send(server.get_fds()[id].fd, ERR_NOSUCHCHANNEL(command[1]).c_str(), ERR_NOSUCHCHANNEL(command[1]).length(), 0);
        return 1;
    }
    
    if(command[2].empty())
    {
        send(server.get_fds()[id].fd, ERR_NOSUCHNICK("empty nickname").c_str(), ERR_NOSUCHNICK("empty nickname").length(), 0);
        return 1;
    }
    try
    {
        id_victim = server.getUserwithNickname(command[2]).returnId();
    }
    catch(const std::exception& e)
    {
        send(server.get_fds()[id].fd, ERR_NOSUCHNICK(command[2]).c_str(), ERR_NOSUCHNICK(command[2]).length(), 0);
        return 1;
    }
    found = false;
    for (vict = (it).getWhiteList().begin(); vict != (it).getWhiteList().end(); ++vict)
    {
        if (*vict == id_victim)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        send(server.get_fds()[id].fd, ERR_USERNOTINCHANNEL(command[2], command[1]).c_str(), ERR_USERNOTINCHANNEL(command[2], command[1]).length(), 0);
        return 1;
    }
    found = false;
    for (std::vector<int>::iterator go = (it).getWhiteList().begin(); go != (it).getWhiteList().end(); ++go)
    {
        if (*go == id_client)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        send(server.get_fds()[id].fd, ERR_NOTONCHANNEL(command[1]).c_str(), ERR_NOTONCHANNEL(command[1]).length(), 0);
        return 1;
    }
    found = false;
    for (std::vector<int>::iterator go = (it).getOperators().begin(); go != (it).getOperators().end(); ++go)
    {
        if (*go == id_client)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        send(server.get_fds()[id].fd, ERR_CHANOPRIVSNEEDED(command[1]).c_str(), ERR_CHANOPRIVSNEEDED(command[1]).length(), 0);
        return 1;
    }
    (it).getWhiteList().erase(vict);
    if(command[3].empty())
        reason = server.getUser(server.get_fds()[id].fd).returnNickname();
    else
        reason = command[3];
    server.send_all(":" + server.getUser(server.get_fds()[id].fd).returnHostname() + " KICK " + command[1] + " " + command[2] + " :" + reason);
    return 1;
}

