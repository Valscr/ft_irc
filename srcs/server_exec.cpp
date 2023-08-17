/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_exec.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyacini <kyacini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 11:02:33 by valentin          #+#    #+#             */
/*   Updated: 2023/08/17 16:12:12 by kyacini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"
#include "../includes/Commands.hpp"
typedef void (*fct)(std::vector<std::string> &, int, Server &);

bool run;
int disconnect(int i, Server &server, bool end)
{
    int id = server.getUser(server.get_fds()[i].fd).returnId();
    if (server.isUser_id(i))
    {
        try {
            server.deleteUser(server.get_fds()[i].fd);
            server.close_fd(i);
            server.erase_fd(i);
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what();
            return (server.freeEverything(), 0);
        }
        if (!end)
            std::cout << "User " << id << " disconnected"<< std::endl;
    }
    return (1);
}

void handleSignal(int signal)
{
    (void)signal;
    if (run == true)
    {
        run = false;
        std::cout << std::endl <<  ANSI_YELLOW << "server closing." << ANSI_RESET << std::endl;
    }
}

void new_user(Server &server)
{
    static int id = 1;
    int nb_fds = server.get_fds().size();
    server.set_Clientsocket(accept(server.getListensocket(), NULL, NULL)); 
    if (server.getClientsocket() < 0) {
        if (errno != EWOULDBLOCK && errno != EAGAIN)
            throw std::runtime_error("not accepted connexion");
    } else {
        if (fcntl(server.getClientsocket(), F_SETFL, O_NONBLOCK) < 0)
            throw std::runtime_error("configuration socket in non-blocking mode");
        server.set_fds_i_fd();
        server.createUser("", id, server.get_fds().back().fd);
        //std::cout << nb_fds << std::endl;
        int fd = server.getUsersList()[nb_fds - 1].returnFd();
		std::cout << "Client " << id << "[" << fd << "]" << " created" << std::endl;
    }
    id++;
}
std::vector<std::string>	split_buffer(int fd, std::string str, Server *server) {
	std::string			line;
	std::vector<std::string>	res;

    server->addmsg_rcv(fd, str);

    std::string input;
    std::istringstream r_iss(server->get_send()[fd]);
    while (std::getline(r_iss, input, '\r')) {
        line.append(input);
    }
    std::istringstream n_iss(line);
    while (std::getline(n_iss, input, '\n')) {
        res.push_back(input);
    }
	return res;
}
std::vector<std::string>	split_command(std::string str) {
	std::vector<std::string>	res;
    int i = 0;
    std::string stock;
    std::string input;
    std::istringstream r_iss(str);
    while (std::getline(r_iss, input, ' '))
    {
        if (!input.empty() && (i == 0))
        {
            res.push_back(input);
        }
        else if (!input.empty() && (i == 1))
        {
            res.push_back(stock);
            stock.erase();
            res.back().append(input);
            i = 0;
        }
        else if (i == 0)
        {
            stock.append(" ");
            i = 1;
        }
        else
        {
            stock.append(" ");
        }
    }
    if (!stock.empty())
    {
        res.back().append(stock);
        res.back().append(" ");
    }
    else if (str[str.length() - 1] == ' ')
    {
        res.back().append(" ");
    }
	return res;
}

int exec_command(std::vector<std::string> command, Server &server, int id, std::map<std::string, Commands::fct> services)
{
    int ret = 1;
    std::map<std::string, Commands::fct>::iterator it = services.find(command[0]);
    if (it != services.end()) {
        Commands cmdObject;
        Commands::fct cmd = it->second;
        ret = (cmdObject.*cmd)(command, id, server);
        if (!server.getChannels().empty())
        {
            for (size_t i = 0; i < server.getChannels().size(); i++)
            {
                Channel *chan = server.getChannels()[i];
                std::cout << "Nom : " << (*chan).getName() << std::endl;
                std::cout << "White list : " << std::endl;
                for (std::vector<int>::iterator it = (*chan).getWhiteList().begin(); it != (*chan).getWhiteList().end(); ++it)
                {
		            std::cout << "fd : " << *it << std::endl;
                }
            }
        }
    } else if (command[0] != "CAP"){
        msg_421();
        return (ret);
    }
    try
    {
    for (std::map<int, std::string>::const_iterator it = server.get_send_client().begin(); it != server.get_send_client().end(); ++it)
    {
        if (!it->second.empty())
        {
            {
                const char* data = it->second.c_str();
                std::cout << ANSI_GREEN << server.getUser(it->first).returnId() << " > " << data << ANSI_RESET << std::endl;
                send(it->first, data, std::strlen(data), 0);
            }
            server.erase_send(it->first);
        }
    }
    }
    catch(const std::runtime_error& e)
    {
        return (ret);
    }
    return (ret);
}

int server_exec(Server &server)
{
    char buffer[BUFFER_SIZE];
    run = true;
    
    signal(SIGINT, handleSignal);
    while (run)
    {
        if (poll(server.get_fds().data(), server.get_fds().size(), -1) < 0)
            handleSignal(1);
        for (size_t i = 0; i < server.get_fds().size(); i++)
        {
            if (server.get_fds()[i].revents)
            {
                if (server.get_fds()[i].fd == server.getListensocket())
                    new_user(server);
                else if (server.get_fds()[i].revents & POLLHUP)
                {
                    if(!disconnect(i, server, false))
                        return 0;
                }
                else 
                {
                    int bytesRead = recv(server.get_fds()[i].fd, buffer, BUFFER_SIZE, 0);
                    if (bytesRead < 0)
                    {
                        if (errno != EWOULDBLOCK && run == true && errno != EAGAIN)
                            throw std::runtime_error("reading buffer");
                    }
                    else if (bytesRead == 0)
                    {
                        if(!disconnect(i, server, false))
                            return 0;
                    }
                    else
                    {
                        buffer[bytesRead] = '\0';
                        std::string received_data(buffer);
                        std::vector<std::string> inputs = split_buffer(server.get_fds()[i].fd, received_data, &server);
                        for (std::vector<std::string>::iterator it = inputs.begin() ; it != inputs.end(); ++it)
                        {
                            std::cout << "------------------------------" << std::endl;
                            std::vector<std::string> command = split_command(*it);
                            std::cout << "\033[96m" << server.getUser(server.get_fds()[i].fd).returnId() << " < " << (*it) << "\033[0m" << std::endl;
                            if ((server.getUser(server.get_fds()[i].fd).returnPassword() == true) || ((command[0] == "PASS") || (command[0] == "CAP")))
                            {
                                if (!exec_command(command, server, i, server.getCommand()->getServices()))
                                    break;
                            }
                            else
                            {
                                disconnect(i, server, true);
                                std::cout << ":password missing" << std::endl;
                                break ;
                            }
                        }
                    }
                }
            }
        }
    }
    server.freeEverything();
    return 1;
}