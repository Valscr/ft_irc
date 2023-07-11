/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 02:25:47 by valentin          #+#    #+#             */
/*   Updated: 2023/07/10 17:04:01 by valentin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string find_next_word(int i, std::string str)
{
    int j = i;
    while (str[i] != '\r' && str[i] != '\0' && str[i] != ' ' && str[i] != '\n')
        i++;
    return (str.substr(j, i - j));
}

std::string find_previous_word(int i, std::string str)
{
    int j = i - 1;
    while (j >= 0 && str[j] != '\r' && str[j] != ' ' && str[j] != '\n')
        j--;
    return str.substr(j + 1, (i - 1) - j);
}

void send_whitelist(Server &server, int fd, std::string channel, std::string buffer)
{
    for (std::vector<int>::iterator it = server.getChannel(channel).getOperators().begin(); it != server.getChannel(channel).getOperators().end(); ++it)
    {
        if (fd != *it)
        {
            server.get_send_fd(*it).append(buffer);
        }
    }
    for (std::vector<int>::iterator it = server.getChannel(channel).getWhiteList().begin(); it != server.getChannel(channel).getWhiteList().end(); ++it)
    {
        if (fd != *it)
        {
            server.get_send_fd(*it).append(buffer);
        }
    }
}

void parse_buffer(std::string buffer, Server &server, int fd)
{
    if (buffer.find("PING") != std::string::npos)
        server.get_send_fd(fd).append(("PONG " + server.get_name() + "\r\n").c_str());
    if (buffer.find("JOIN #") != std::string::npos)
        join_funct(buffer, server, fd);
    if (buffer.find("PRIVMSG") != std::string::npos)
        privmsg_funct(buffer, server, fd);
    if (buffer.find("WHO #") != std::string::npos)
    {
        std::vector<int> oper = server.getChannel(find_next_word(5, buffer)).getOperators();
        for (std::vector<int>::iterator it = oper.begin(); it != oper.end(); ++it)
	    {
            if (fd != *it)
		        server.get_send_fd(fd).append((":" + std::string(SERVER_NAME) + " 352 " + server.getUser(fd).returnNickname() + " " + find_next_word(4, buffer) + " " + server.getUser(*it).returnUsername() + " " + server.getUser(*it).returnHostname() + " " + std::string(SERVER_NAME) + " " + server.getUser(*it).returnNickname() + " H :0 " + server.getUser(*it).returnRealname() + "\r\n").c_str());
        }
        std::vector<int> whitelist = server.getChannel(find_next_word(5, buffer)).getWhiteList();
        for (std::vector<int>::iterator it = whitelist.begin(); it != whitelist.end(); ++it)
	    {
            if (fd != *it)
		        server.get_send_fd(fd).append((":" + std::string(SERVER_NAME) + " 352 " + server.getUser(fd).returnNickname() +  " " + find_next_word(4, buffer) + " " + server.getUser(*it).returnUsername() + " " + server.getUser(*it).returnHostname() + " " + std::string(SERVER_NAME) + " " + server.getUser(*it).returnNickname() + " V :0 " + server.getUser(*it).returnRealname() + "\r\n").c_str());
        }
        server.get_send_fd(fd).append((":" + std::string(SERVER_NAME) + " 315 " + server.getUser(fd).returnNickname() + " " + find_next_word(4, buffer) + " :End of WHO list\r\n").c_str());
    }
}

void send_function(Server &server, std::vector<pollfd> fds)
{
    for (std::map<int, std::string>::const_iterator it = server.get_send().begin(); it != server.get_send().end(); ++it)
    {
        if (!it->second.empty())
        {
            {
                const char* data = it->second.c_str();
                std::cout << ANSI_GREEN << MAX_CLIENTS - it->first + 1 << " > " << data << ANSI_RESET << std::endl;
                send(fds[it->first].fd, data, std::strlen(data), 0);
            }
            server.erase_send(it->first);
        }
    }
}
