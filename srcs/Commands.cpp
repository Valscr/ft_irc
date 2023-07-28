#include "../includes/Commands.hpp"
#include "../includes/Server.hpp"
#include "../includes/irc.hpp"


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

int Commands::PASS(std::vector<std::string> &command, int i, Server &server)
{
    int fd = server.get_fds()[i].fd;
    if (command.size() == 1)
    {
        server.addmsg_send(fd, ERR_UNKNOWNCOMMAND(command[0]));
        disconnect(i, server, false);
        return 0;
    }
    if (!server.getUser(fd).returnPassword() && (command[1] == server.get_password()))
    {
        server.getUser(fd).setPassword(true);
        return (1);
    }
    else if(server.getUser(fd).returnPassword())
    {
        server.addmsg_send(fd, ERR_ALREADYREGISTERED);
        return (1);
    }
    else
    {
        server.addmsg_send(fd, ERR_PASSWDMISMATCH);
        disconnect(i, server, false);
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
int Commands::NICK(std::vector<std::string> &command, int i, Server &server)
{
    int fd = server.get_fds()[i].fd;
    if (command[1].empty())
    {
        server.addmsg_send(fd,ERR_NONICKNAMEGIVEN);
        disconnect(i, server, false);
        return 0;
    }
    else if (!validNickname(command[1]))
    {
        server.addmsg_send(fd,ERR_ERRONEUSNICKNAME(command[1]));
        disconnect(i, server, false);
        return 0;
    }
    else if(server.NicknameMatching(command[1]))
    {
        server.addmsg_send(fd, ERR_NICKNAMEISUSE(command[1]));
        disconnect(i, server, false);
        return 0;
    }
    server.getUser(fd).setNickname(command[1]);
    server.getUser(fd).setHasNickname(true);
    //send(fd, msg_001(server.getUser(fd).returnNickname()).c_str(), msg_001(server.getUser(fd).returnNickname()).length(), 0);
    return (1);
}

std::string ft_concatener(std::vector<std::string> command, size_t start, size_t end, std::string txt)
{
    for (size_t i = start; i < end; i++) {
        if (!command[i].empty())
        {
            txt += " ";
            txt += command[i];
        }
    }
    return (txt);
}
int Commands::USER(std::vector<std::string> &command, int i, Server &server)
{
    int fd = server.get_fds()[i].fd;
    int id = server.getUser(fd).returnId();

    if (command.size() < 5)
    {
        
        disconnect(i, server, false);
        return 0;
    }
    else if(server.getUser(fd).returnRegistered())
    {
        server.addmsg_send(fd,ERR_ALREADYREGISTERED);
        return 1;
    }
    if (server.UserExist_fd(fd) && server.getUser(fd).returnHasNickname())
    {
        server.getUser(fd).setUsername(command[1]);
        server.getUser(fd).setHotsname(command[2]);
        server.getUser(fd).setRealsname(ft_concatener(command, 5, command.size(), command[4].substr(1)));
        server.getUser(fd).setRegistered(true);
        std::cout << "Client " << id << "[" << fd << "]" << " connected" << std::endl;
        server.addmsg_send(fd,RPL_WELCOME(server.getUser(fd).returnNickname(),
            server.getUser(fd).returnUsername(), server.getUser(fd).returnHostname()));
        return (1);
    }
    else
    {
        std::cout << "User not found" << std::endl;
        return (0);
    }
}

int Commands::PING(std::vector<std::string> &command, int i, Server &server)
{
    int fd = server.get_fds()[i].fd;
    std::string msg = "PONG :" + command[1] + "\r\n";
    server.addmsg_send(fd, msg.c_str());
    return (1);
}

std::vector<std::string>	split_names(std::string str) {
	std::vector<std::string>	res;

    std::string input;
    std::istringstream r_iss(str);
    while (std::getline(r_iss, input, ',')) {
        res.push_back(input);
    }
	return res;
}

int Commands::JOIN(std::vector<std::string> &command, int j, Server &server)
{
    std::vector<std::string> chanPasswords;
    int fd = server.get_fds()[j].fd;
    int id = server.getUser(j).returnId();
	if (command.size() < 2)
    {
        server.addmsg_send(fd,ERR_NEEDMOREPARAMS(command[0]));
		return (1);
	}

    std::vector<std::string> chanNames = split_names(command[1]);
    if (command.size() > 2)
        chanPasswords = split_names(command[2]);
    
    for (size_t i = 0; i < chanNames.size(); i++)
    {
        if (!(chanNames[i][0] == '#') || (chanNames[i][0] == '&'))
        {
            server.addmsg_send(fd,ERR_BADCHANMASK(chanNames[i]));
            continue ;
        }
        if (!server.find_channel(chanNames[i]))
        {
            if((command.size() > 2) && (chanPasswords.size() >= (i + 1)))
                server.createChannel(chanNames[i], fd, chanPasswords[i]);
            else
                server.createChannel(chanNames[i], fd, "");
        }
        Channel chan = server.getChannel(chanNames[i]);
        if (chan.getInviteMode())
        {
            if (!chan.isInvited(fd))
            {
                //ERR_INVITEONLYCHAN
                continue;
            }
        }
        //verifier si le chan necessite un mdp
        if (!(chan.getHavePassword() || (chan.getPassword() == chanPasswords[i])))
        {
            server.addmsg_send(fd, ERR_BADCHANNELKEY(server.getUser(id).returnNickname(), chanNames[i]));
            continue;
        }
        if (!chan.alreadyExist(&(server.getUser(fd))))
        {
            if (chan.getHasLimit() && (chan.getLimit() < int(chan.getUsers().size() + 1)))
            {
                server.addmsg_send(fd, ERR_CHANNELISFULL(server.getUser(id).returnNickname(), chanNames[i]));
                continue;
            }
            chan.addUser(&(server.getUser(fd)));
            //envoyer au client les infos !
        }
    }
    return (1);
}

/***********************************************************************/
/*                          GETTERS & SETTERS                         */
/*********************************************************************/

std::map<std::string, fct> Commands::getServices()
{
    return (this->services_list);
}
