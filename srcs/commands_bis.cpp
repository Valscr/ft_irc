/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_bis.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 00:40:50 by valentin          #+#    #+#             */
/*   Updated: 2023/07/28 11:49:23 by valentin         ###   ########.fr       */
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
        if (server.find_channel(find_next_word(9, buffer)) && server.getChannel(find_next_word(9, buffer)).find_user_channels(server.get_fds()[id].fd))
            send_whitelist(server, server.get_fds()[id].fd, find_next_word(9, buffer), (":" + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + buffer + "\r\n").c_str());
        else if (server.find_channel(find_next_word(9, buffer)) && !server.getChannel(find_next_word(9, buffer)).find_user_channels(id))
            server.get_send_fd(server.getUserwithNickname(client[2]).returnFd()).append((ERR_CANNOTSENDTOCHAN(server.getUser(server.get_fds()[id].fd).returnNickname(), client[1])).c_str());
        else
            server.get_send_fd(server.getUserwithNickname(client[2]).returnFd()).append((ERR_NOSUCHNICK(client[1])).c_str());
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

int		Commands::TOPIC(std::vector<std::string> &client, int id, Server &server)
{
    std::string buffer;

    for (size_t i = 2; i < client.size(); ++i) {
        buffer += client[i];
        if (i < client.size() - 1) {
            buffer += " ";
        }
    }
    if (server.find_channel(client[1].substr(1)) && (server.getChannel(client[1].substr(1)).is_operator(server.get_fds()[id].fd) || !server.getChannel(client[1].substr(1)).getTopicMode()))
    {
        server.getChannel(client[1].substr(1)).setTopic(buffer.substr(1));
        send_whitelist(server, server.get_fds()[id].fd, client[1].substr(1), (RPL_TOPIC(server.getUser(server.get_fds()[id].fd).returnNickname(), client[1].substr(1), buffer.substr(1)).c_str()));
        server.get_send_fd(server.get_fds()[id].fd).append((RPL_TOPIC(server.getUser(server.get_fds()[id].fd).returnNickname(), client[1].substr(1), buffer.substr(1)).c_str()));
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
    if (server.find_channel(find_next_word(6, buffer)) && (!server.getChannel(find_next_word(6, buffer)).getlimitMode()
        || server.getChannel(find_next_word(6, buffer)).getUserlimit() > server.getChannel(find_next_word(6, buffer)).count_user()))
    {
        if ((server.getChannel(find_next_word(6, buffer)).getInviteMode() && server.getUser(server.get_fds()[id].fd).is_invite_channel(find_next_word(6, buffer)))
            || !server.getChannel(find_next_word(6, buffer)).getInviteMode())
        {
            server.getChannel(find_next_word(6, buffer)).addWhiteList(server.get_fds()[id].fd);
            send_whitelist(server, server.get_fds()[id].fd, find_next_word(6, buffer), (":" + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + buffer).c_str());
            server.get_send_fd(server.get_fds()[id].fd).append((":" + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + buffer).c_str());
            if (!server.getChannel(find_next_word(6, buffer)).getTopic().empty())
                server.get_send_fd(server.get_fds()[id].fd).append(RPL_TOPIC(server.getUser(server.get_fds()[id].fd).returnNickname(), find_next_word(6, buffer), server.getChannel(find_next_word(6, buffer)).getTopic()).c_str());
            server.get_send_fd(server.get_fds()[id].fd).append(RPL_NAMREPLY_BIS(server.getUser(server.get_fds()[id].fd).returnNickname(), find_next_word(6, buffer), msg_353(server, find_next_word(6, buffer))).c_str());
            server.get_send_fd(server.get_fds()[id].fd).append(RPL_ENDOFNAMES(server.getUser(server.get_fds()[id].fd).returnNickname(), find_next_word(6, buffer)).c_str());
        }
        else
            server.get_send_fd(server.get_fds()[id].fd).append(ERR_INVITEONLYCHAN(server.getUser(server.get_fds()[id].fd).returnNickname(), find_next_word(6, buffer)).c_str());

    }
    else if (server.find_channel(find_next_word(6, buffer)) && server.getChannel(find_next_word(6, buffer)).getlimitMode()
        && server.getChannel(find_next_word(6, buffer)).getUserlimit() <= server.getChannel(find_next_word(6, buffer)).count_user())
        server.get_send_fd(server.get_fds()[id].fd).append(ERR_CHANNELISFULL(server.getUser(server.get_fds()[id].fd).returnNickname(), find_next_word(6, buffer)).c_str());
    else if (!server.find_channel(find_next_word(6, buffer)))
    {
        server.createChannel(find_next_word(6, buffer), server.get_fds()[id].fd);
        server.get_send_fd(server.get_fds()[id].fd).append((":" + server.getUser(server.get_fds()[id].fd).returnNickname() + " " + buffer).c_str());
    }
    return 1;
}

int		Commands::INVITE(std::vector<std::string> &client, int id, Server &server)
{
    if (server.find_channel(client[2].substr(1)) && server.UserExist(client[1]))
    {
        server.get_send_fd(server.getUserwithNickname(client[1]).returnFd()).append((":" + server.getUser(server.get_fds()[id].fd).returnNickname() + " INVITE " + client[1] + " " + client[2] + "\r\n").c_str());
        server.getUserwithNickname(client[1]).addInviteChannel(client[2].substr(1));
    }
    return (1);
}

int		Commands::MODE(std::vector<std::string> &client, int id, Server &server)
{
    (void)id;
    if (server.find_channel(client[1].substr(1)) && server.getChannel(client[1].substr(1)).is_operator(server.get_fds()[id].fd))
    {
        if (client[2].find("+i") != std::string::npos && server.find_channel(client[1].substr(1)))
            server.getChannel(client[1].substr(1)).setInviteMode(true);
        if (client[2].find("-i") != std::string::npos && server.find_channel(client[1].substr(1)))
            server.getChannel(client[1].substr(1)).setInviteMode(false);
        if (client[2].find("+t") != std::string::npos && server.find_channel(client[1].substr(1)))
            server.getChannel(client[1].substr(1)).setTopicRestriction(true);
        if (client[2].find("-t") != std::string::npos && server.find_channel(client[1].substr(1)))
            server.getChannel(client[1].substr(1)).setTopicRestriction(false);
        if (client[2].find("+o") != std::string::npos && server.find_channel(client[1].substr(1)))
            server.getChannel(client[1].substr(1)).addOperator(server.getUserwithNickname(client[3]).returnFd());
        if (client[2].find("-o") != std::string::npos && server.find_channel(client[1].substr(1)))
            server.getChannel(client[1].substr(1)).removeOperator(server.getUserwithNickname(client[3]).returnFd());
        if (client[2].find("+l") != std::string::npos && server.find_channel(client[1].substr(1)))
        {
            server.getChannel(client[1].substr(1)).setlimitMode(true);
            server.getChannel(client[1].substr(1)).setUserLimit(std::stoi(client[3]));
        }
        if (client[2].find("-l") != std::string::npos && server.find_channel(client[1].substr(1)))
            server.getChannel(client[1].substr(1)).setlimitMode(false);
    }
    return (1);
}