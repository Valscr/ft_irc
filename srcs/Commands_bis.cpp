/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands_bis.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 00:40:50 by valentin          #+#    #+#             */
/*   Updated: 2023/08/30 20:01:03 by valentin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"
#include "../includes/msg.hpp"

/***********************************************************************/
/*                          Val's space                               */
/*********************************************************************/

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
        if (server.find_channel(find_next_word(8, buffer)) && server.getChannel(find_next_word(8, buffer))->find_channels(server.get_fds()[id].fd))
            send_whitelist(server, server.get_fds()[id].fd, find_next_word(8, buffer), (":" + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + buffer + "\r\n").c_str());
        else if (server.find_channel(find_next_word(8, buffer)) && !server.getChannel(find_next_word(8, buffer))->find_channels(id))
            server.get_send_fd(server.get_fds()[id].fd).append((ERR_CANNOTSENDTOCHAN(server.getUser(server.get_fds()[id].fd).returnNickname(), client[1])).c_str());
        else
            server.get_send_fd(server.get_fds()[id].fd).append((ERR_NOSUCHNICK(client[1])).c_str());
    }
    else
    {
        if (server.UserExist(client[1]))
            server.get_send_fd(server.getUserwithNickname(client[1]).returnFd()).append((":" + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + buffer + "\r\n").c_str());
        else
            server.get_send_fd(server.get_fds()[id].fd).append((RPL_AWAY(server.getUser(server.get_fds()[id].fd).returnNickname(), client[1])).c_str());
    }
    return 1;
}

int		Commands::NOTICE(std::vector<std::string> &client, int id, Server &server)
{
    std::string buffer;

    for (size_t i = 0; i < client.size(); ++i) {
        buffer += client[i];
        if (i < client.size() - 1) {
            buffer += " ";
        }
    }
    if (buffer.find("NOTICE #") != std::string::npos)
    {
        if (server.find_channel(find_next_word(7, buffer)) && server.getChannel(find_next_word(7, buffer))->find_channels(server.get_fds()[id].fd))
            send_whitelist(server, server.get_fds()[id].fd, find_next_word(7, buffer), (":" + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + buffer + "\r\n").c_str());
    }
    else
    {
        if (server.UserExist(client[1]))
            server.get_send_fd(server.getUserwithNickname(client[1]).returnFd()).append((":" + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + buffer + "\r\n").c_str());
    }
    return 1;
}

int		Commands::QUIT(std::vector<std::string> &command, int id, Server &server)
{
    (void)command;
    disconnect(id, server, false);
    return (1);
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
    if (server.find_channel(client[1]) && (server.getChannel(client[1])->is_operator(server.get_fds()[id].fd) || !server.getChannel(client[1])->getTopicMode()))
    {
        server.getChannel(client[1])->setTopic(buffer.substr(1));
        send_whitelist(server, server.get_fds()[id].fd, client[1], (RPL_TOPIC_BIS(server.getUser(server.get_fds()[id].fd).returnNickname(), client[1].substr(1), buffer.substr(1)).c_str()));
        server.get_send_fd(server.get_fds()[id].fd).append((RPL_TOPIC_BIS(server.getUser(server.get_fds()[id].fd).returnNickname(), client[1].substr(1), buffer.substr(1)).c_str()));
    }
    return (1);
}

int		Commands::INVITE(std::vector<std::string> &client, int id, Server &server)
{
    if (client.size() != 3)
        return 0;
    if (server.find_channel(client[2]) && server.UserExist(client[1]))
    {
        server.get_send_fd(server.getUserwithNickname(client[1]).returnFd()).append((":" + server.getUser(server.get_fds()[id].fd).returnNickname() + " INVITE " + client[1] + " " + client[2] + "\r\n").c_str());
        server.getChannel(client[2])->addUser(server.getUserwithNickname(client[1]).returnFd());
    }
    return (1);
}

int		Commands::MODE(std::vector<std::string> &client, int id, Server &server)
{
    if (server.find_channel(client[1]) && client.size() > 2 && server.getChannel(client[1])->is_operator(server.get_fds()[id].fd))
    {
        if (client[2][0] == '+' && client[2][1] == 'i' && client[2].length() < 3 && server.find_channel(client[1]))
            server.getChannel(client[1])->setInviteMode(true);
        if (client[2][0] == '-' && client[2][1] == 'i' && client[2].length() < 3 && server.find_channel(client[1]))
            server.getChannel(client[1])->setInviteMode(false);
        if (client[2][0] == '+' && client[2][1] == 't' && client[2].length() < 3 && server.find_channel(client[1]))
            server.getChannel(client[1])->setTopicRestriction(true);
        if (client[2][0] == '-' && client[2][1] == 't' && client[2].length() < 3 && server.find_channel(client[1]))
            server.getChannel(client[1])->setTopicRestriction(false);
        if (client[2][0] == '+' && client[2][1] == 'o' && client[2].length() < 3 && server.find_channel(client[1]))
        {
            server.getChannel(client[1])->addOperator(server.getUserwithNickname(client[3]).returnFd());
        }
        if (client[2][0] == '-' && client[2][1] == 'o' && client[2].length() < 3 && server.find_channel(client[1]))
            server.getChannel(client[1])->removeOperator(server.getUserwithNickname(client[3]).returnFd());
        if (client[2][0] == '+' && client[2][1] == 'l' && client[2].length() < 3 && server.find_channel(client[1]))
        {
            server.getChannel(client[1])->setlimitMode(true);
            server.getChannel(client[1])->setLimit(std::atoi(client[3].c_str()));
        }
        if (client[2][0] == '-' && client[2][1] == 'l' && client[2].length() < 3 && server.find_channel(client[1]))
            server.getChannel(client[1])->setlimitMode(false);
        if (client[2][0] == '+' && client[2][1] == 'k' && client[2].length() < 3 && server.find_channel(client[1]) && client.size() == 4)
        {
            server.getChannel(client[1])->setPassMode(true);
            server.getChannel(client[1])->setPassword(client[3]);

        }
        if (client[2][0] == '-' && client[2][1] == 'k' && client[2].length() < 3 && server.find_channel(client[1]))
        {
            server.getChannel(client[1])->setPassMode(false);
        }
            
    }
    return (1);
}