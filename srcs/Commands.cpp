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

int		Commands::PRIVMSG(std::vector<std::string> &client, int id, Server &server)
{
    std::string buffer;

    for (size_t i = 0; i < client.size(); ++i) {
        buffer += client[i];
        if (i < client.size() - 1) {
            buffer += " ";
        }
    }
    if (buffer.find("PRIVMSG #") != std::string::npos)
    {
        if (server.find_channel(find_next_word(9, buffer)) && server.getChannel(find_next_word(9, buffer)).find_user_channels(server.get_fds()[id].fd) && !server.getChannel(find_next_word(9, buffer)).is_ban(server.get_fds()[id].fd))
            send_whitelist(server, server.get_fds()[id].fd, find_next_word(9, buffer), (":" + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + buffer + "\r\n").c_str());
        else if (server.find_channel(find_next_word(9, buffer)) && !server.getChannel(find_next_word(9, buffer)).find_user_channels(id))
        {
            server.get_send_fd(server.getUserwithNickname(client[2]).returnFd()).append((":" + std::string(SERVER_NAME) + " 404 " + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + client[1] + " :Cannot send message to this channel\r\n").c_str());
        }
        else
            server.get_send_fd(server.getUserwithNickname(client[2]).returnFd()).append((":" + std::string(SERVER_NAME) + " 401 " + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + client[1] + " :No such channel\r\n").c_str());
    }
    else
    {
        if (server.UserExist(client[1]))
            server.get_send_fd(server.getUserwithNickname(client[1]).returnFd()).append((":" + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + buffer + "\r\n").c_str());
        else
        {
            server.get_send_fd(server.get_fds()[id].fd).append((":" + std::string(SERVER_NAME) + " 301 " + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + client[1] + " :This user is disconnected.\r\n").c_str());
        }
    }
    return 1;
}

int		Commands::TOPIC(std::vector<std::string> &client, int id, Server &server)
{
    std::string buffer;

    for (size_t i = 2; i < client.size(); ++i) {
        buffer += client[i];
        if (i < client.size() - 1) {
            buffer += " ";
        }
    }
    if (server.find_channel(client[1].substr(1)) && server.getChannel(client[1].substr(1)).is_operator(server.get_fds()[id].fd))
    {
        send_whitelist(server, server.get_fds()[id].fd, client[1].substr(1), (":" + std::string(SERVER_NAME) + " 332 " + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + client[1].substr(1) + " :" + buffer.substr(1) + "\r\n").c_str());
        server.get_send_fd(server.get_fds()[id].fd).append((":" + std::string(SERVER_NAME) + " 332 " + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + client[1].substr(1) + " :" + buffer.substr(1) + "\r\n").c_str());
    }
    return (1);
}

int		Commands::JOIN(std::vector<std::string> &client, int id, Server &server)
{
    std::string buffer;

    for (size_t i = 0; i < client.size(); ++i) {
        buffer += client[i];
        if (i < client.size() - 1) {
            buffer += " ";
        }
    }
    buffer += "\r\n";
    if (server.find_channel(find_next_word(6, buffer)) && !server.getChannel(find_next_word(6, buffer)).is_ban(id))
    {
        server.getChannel(find_next_word(6, buffer)).addWhiteList(server.get_fds()[id].fd);
        send_whitelist(server, server.get_fds()[id].fd, find_next_word(6, buffer), (":" + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + buffer).c_str());
        server.get_send_fd(server.get_fds()[id].fd).append((":" + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + buffer).c_str());
        server.get_send_fd(server.get_fds()[id].fd).append((":" + std::string(SERVER_NAME) + " 332 " + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + find_next_word(6, buffer) + " :Channel topic here\r\n").c_str());
        server.get_send_fd(server.get_fds()[id].fd).append((":" + std::string(SERVER_NAME) + " 353 " + server.getUser(server.get_fds()[id].fd).returnNickname() +  " = #" + find_next_word(6, buffer) + " :" + msg_353(server, find_next_word(6, buffer)) + "\r\n").c_str());
        server.get_send_fd(server.get_fds()[id].fd).append((":" + std::string(SERVER_NAME) + " 366 " + server.getUser(server.get_fds()[id].fd).returnNickname() + " #" + find_next_word(6, buffer) + " :End of NAMES list\r\n").c_str());
    }
    else if (server.find_channel(find_next_word(6, buffer)) && server.getChannel(find_next_word(6, buffer)).is_ban(id))
        server.get_send_fd(server.get_fds()[id].fd).append((":" + std::string(SERVER_NAME) + " 403 " + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + find_next_word(5, buffer) + " :" + "tou are banned from " + find_next_word(6, buffer) + "\r\n").c_str());
    else
    {
        server.createChannel(find_next_word(6, buffer), server.get_fds()[id].fd);
        server.get_send_fd(server.get_fds()[id].fd).append((":" + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + buffer).c_str());
        server.get_send_fd(server.get_fds()[id].fd).append((":" + std::string(SERVER_NAME) + " 332 " + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + find_next_word(6, buffer) + " :Channel topic here\r\n").c_str());
    }
    return 1;
}