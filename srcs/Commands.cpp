#include "../includes/Commands.hpp"
#include "../includes/Server.hpp"
#include "../includes/irc.hpp"

Commands::Commands(void) 
{
    this->services_list = get_services();    
    return;
}
Commands::~Commands(void) { return; }

void Commands::PASS(std::vector<std::string> &command, int id, Server &server)
{
    User user = server.getUsersList()[id - 1];
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
        server.close_fd(server.get_fds()[id].fd);
        std::cout << "User " << id << " disconnected\n" << std::endl;
    }
}

                       /* std::cout << "je teste si le mdp est correct" << server.get_password() << std::endl;
                         std::cout << find_next_word(buffer.find("PASS") + 5, buffer) << std::endl;
                        if (server.get_password() == find_next_word(buffer.find("PASS") + 5, buffer))
                        {
                            password[i] = true;
                        }
                        else
                        {
                            send(server.get_fds()[i].fd, msg_464().c_str(), msg_464().length(), 0);
                            std::cout << "> " << msg_464().c_str() << std::endl;
                        }*/