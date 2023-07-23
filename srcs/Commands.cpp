#include "../includes/Commands.hpp"
#include "../includes/Server.hpp"
#include "../includes/irc.hpp"


/***********************************************************************/
/*                   CONSTRUCTOR & DESTRUCTOR                         */
/*********************************************************************/

typedef void (Commands::*fct)(std::vector<std::string> &, int, Server &);
Commands::Commands(void) 
{
    this->services_list = setServices();    
    return;
}
Commands::~Commands(void) { return; }


/***********************************************************************/
/*                         COMMANDS METHODS                           */
/*********************************************************************/

void Commands::PASS(std::vector<std::string> &command, int id, Server &server)
{
    int fd = server.get_fds()[id].fd;
    if (command.size() == 1)
    {
        msg_421();
        disconnect(id, server, false);
        return ;
    }
    if (!server.getUser(fd).returnPassword() && (command[1] == server.get_password()))
    {
        server.getUser(fd).setPassword(true);
    }
    else if(server.getUser(fd).returnPassword())
    {
        msg_462();
    }
    else
    {
        msg_464();
        disconnect(id, server, false);
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
void Commands::NICK(std::vector<std::string> &command, int id, Server &server)
{
    int fd = server.get_fds()[id].fd;
    if (command[1].empty())
    {
        msg_431();
        disconnect(id, server, false);
        return ;
    }
    else if (!validNickname(command[1]))
    {
        msg_432();
        disconnect(id, server, false);
        return ;
    }
    else if(server.NicknameMatching(command[1]))
    {
        msg_433();
        disconnect(id, server, false);
        return ;
    }
    send(fd, msg_001(server.getUser(fd).returnNickname()).c_str(), msg_001(server.getUser(fd).returnNickname()).length(), 0);
    server.getUser(fd).setNickname(command[1]);
    server.getUser(fd).setHasNickname(true);
    std::cout << ANSI_GREEN << MAX_CLIENTS + 1 - id << " > " << msg_001(server.getUser(fd).returnNickname()).c_str() << ANSI_RESET << std::endl;
}

void Commands::USER(std::vector<std::string> &command, int id, Server &server)
{
    int fd = server.get_fds()[id].fd;

    if (command.size() < 5)
    {
        msg_421();
        disconnect(id, server, false);
        return ;
    }
    else if(server.getUser(fd).returnRegistered())
    {
        msg_462();
        return ;
    }
    if (server.UserExist_fd(fd) && server.getUser(fd).returnHasNickname())
    {
        server.getUser(fd).setUsername(command[1]);
        server.getUser(fd).setRealsname(command[2]);
        server.getUser(fd).setHotsname(command[4]);
        server.getUser(fd).setRegistered(true);
        int new_id = server.getUser(fd).returnId();
        std::cout << "Client " << new_id << "[" << fd << "]" << " connected" << std::endl;
    }
    else
        std::cout << "User not found" << std::endl;
}

void Commands::PING(std::vector<std::string> &command, int id, Server &server)
{
    int fd = server.get_fds()[id].fd;
    std::string msg = "PONG :" + command[1] + "\r\n";
    send(fd, msg.c_str(), msg.length(), 0);
    std::cout << ANSI_GREEN << server.getUser(server.get_fds()[id].fd).returnId() << " > " << msg.c_str() << ANSI_RESET << std::endl;
}
/***********************************************************************/
/*                          GETTERS & SETTERS                         */
/*********************************************************************/

std::map<std::string, fct> Commands::getServices()
{
    return (this->services_list);
}