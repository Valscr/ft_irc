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
    User user = server.getUsersList()[id - 1];
    if (command.size() == 1)
    {
        msg_421();
        return ;
    }
    if (!user.returnPassword() && (command[1] == server.get_password()))
    {
        user.setPassword(true);
    }
    else if(user.returnPassword())
    {
        msg_462();
    }
    else
    {
        msg_464();
        //fonction de deconnexion à coder
        //server.close_fd(server.get_fds()[id].fd);
        std::cout << "User " << id << " disconnected\n" << std::endl;
    }
}

/***********************************************************************/
/*                          GETTERS & SETTERS                         */
/*********************************************************************/

std::map<std::string, fct> Commands::getServices()
{
    return (this->services_list);
}