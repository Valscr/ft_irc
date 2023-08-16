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

bool spaceChecker(std::string name)
{
    for (size_t i = 0; i < name.length(); i++)
    {
        if ((name[i] == ' ') || (name[i] == '\n') || (name[i] == '\t'))
            return (true);
    }
    return (false);
}

int Commands::JOIN(std::vector<std::string> &command, int j, Server &server)
{
    static int tmp = 1;
    std::vector<std::string> chanPasswords;
    Channel *chan = NULL;
    int fd = server.get_fds()[j].fd;
    std::cout << " Parametre de JOIN : " << std::endl;
    for (size_t i = 0; i < command.size(); i++)
    {
        std::cout << i << " " << command[i] << std::endl;
    }
	if (command.size() < 2)
    {
        server.addmsg_send(fd,ERR_NEEDMOREPARAMS(command[0]));
		return (1);
	}
    //verifier pour JOIN #
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
        if (spaceChecker(chanNames[i]))
        {
            server.addmsg_send(fd, ERR_NOSUCHCHANNEL(chanNames[i]));
            continue;
        }
        if (!server.find_channel(chanNames[i]))
        {
            if((command.size() > 2) && (chanPasswords.size() >= (i + 1)))
                chan = server.createChannel(chanNames[i], fd, chanPasswords[i], tmp);
            else
                chan = server.createChannel(chanNames[i], fd, "", tmp);
            (*chan).addOperator(fd);
        }
        else
        {
            chan = server.getChannel(chanNames[i]);
        }
        if ((*chan).getInviteMode())
        {
            if (!(*chan).isInvited(fd))
            {
                server.addmsg_send(fd, ERR_INVITEONLYCHAN(server.getUser(fd).returnNickname(), chanNames[i]));
                continue;
            }
        }
        //verifier si le chan necessite un mdp
        //verifier les espaces dans le nom du channel
        if ((*chan).getHavePassword() && ((*chan).getPassword() != chanPasswords[i]))
        {
            server.addmsg_send(fd, ERR_BADCHANNELKEY(server.getUser(fd).returnNickname(), chanNames[i]));
            continue;
        }
        if (!(*chan).alreadyExist(fd))
        {
            if ((*chan).getHasLimit() && ((*chan).getLimit() < int((*chan).getWhiteList().size() + 1)))
            {
                server.addmsg_send(fd, ERR_CHANNELISFULL(server.getUser(fd).returnNickname(), chanNames[i]));
                continue;
            }
        }
        (*chan).addUser(fd);
         server.send_all((":" + server.getUser(fd).returnNickname() + "!"
             + server.getUser(fd).returnUsername() + "@" + server.getUser(fd).returnHostname()  + " JOIN " + chanNames[i] + "\r\n"), *chan);
        if ((*chan).getTopic().empty())
            server.addmsg_send(fd, RPL_TOPIC(server.getUser(fd).returnNickname(), chanNames[i], (*chan).getTopic()));
        else
            server.addmsg_send(fd, RPL_NOTOPIC(server.getUser(fd).returnNickname(), chanNames[i]));
        server.addmsg_send(fd, RPL_NAMREPLY(server.getUser(fd).returnNickname(),
            server.getUser(fd).returnUsername(), server.getUser(fd).returnHostname(), (*chan).getName(), (*chan).getListUsers(server)));
        server.addmsg_send(fd, RPL_ENDOFNAMES(server.getUser(fd).returnNickname(), server.getUser(fd).returnUsername(), "localhost", chanNames[i]));
    }

    tmp++;
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
    int id_client = server.get_fds()[id].fd;
    std::vector<int>::iterator vict;
    std::string reason;
    Channel *it;
    int id_victim;
    bool found = false;

    if(command[1].empty())
    {
        std::cout << "je suis present" << std::endl;
        server.addmsg_send(server.get_fds()[id].fd, ERR_NOSUCHCHANNEL("Empty name").c_str());
        return 1;
    }
    try
    {
        std::cout << "je suis là" << std::endl;
       it = server.getChannel(command[1]);
    }
    catch(const std::exception& e)
    {
        std::cout << "je suis ici" << std::endl;
        server.addmsg_send(server.get_fds()[id].fd, ERR_NOSUCHCHANNEL(command[1]).c_str());
        return 1;
    }
    
    if(command[2].empty())
    {
        std::cout << "je suis ailleurs" << std::endl;
        server.addmsg_send(server.get_fds()[id].fd, ERR_NOSUCHNICK("empty nickname").c_str());
        return 1;
    }
    try
    {
        std::cout << "je suis partout" << std::endl;
        id_victim = server.getUserwithNickname(command[2]).returnFd();
    }
    catch(const std::exception& e)
    {
        std::cout << "je suis invisible" << std::endl;
        server.addmsg_send(server.get_fds()[id].fd, ERR_NOSUCHNICK(command[2]).c_str());
        return 1;
    }
    found = false;
    for (vict = (*it).getWhiteList().begin(); vict != (*it).getWhiteList().end(); ++vict)
    {
        if (*vict == id_victim)
        {
            std::cout << "j'existe" << std::endl;
            found = true;
            break;
        }
    }
    std::cout << " *vict et id_victim :" << *vict << " " << id_victim << std::endl;
    std::cout << " id_client :" << id_client << std::endl;
    if (!found)
    {
        server.addmsg_send(server.get_fds()[id].fd, ERR_USERNOTINCHANNEL(command[2], command[1]).c_str());
    }
    found = false;
    for (std::vector<int>::iterator go = (*it).getWhiteList().begin(); go != (*it).getWhiteList().end(); ++go)
    {
        if (*go == id_client)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        server.addmsg_send(server.get_fds()[id].fd, ERR_NOTONCHANNEL(command[1]).c_str());
        return (1);
    }
    found = false;
    for (std::vector<int>::iterator go = (*it).getOperators().begin(); go != (*it).getOperators().end(); ++go)
    {
        if (*go == id_client)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        server.addmsg_send(server.get_fds()[id].fd, ERR_CHANOPRIVSNEEDED(command[1]).c_str());
        return (1);
    }
    std::vector<int> op = (*it).getOperators();
    std::vector<int>::iterator itVictim = std::find(op.begin(), op.end(), *vict);
    if (itVictim != op.end())
        op.erase(itVictim);
    (*it).getWhiteList().erase(vict);
    if((command.size() < 5) )
    {
        reason = server.getUser(id_client).returnNickname();
        std::cout << "empty reason : " << reason << std::endl;
    }
    else
        reason = ft_concatener(command, 4, 5, command[3].substr(1));
    std::cout << "reason : " << reason << std::endl;
    server.send_all(":" + server.getUser(id_client).returnNickname() + "!"
        + server.getUser(id_client).returnUsername() + "@localhost"
        + " KICK " + command[1] + " " + command[2] + " :" + reason + "\r\n", *it);
    return (1);
}
