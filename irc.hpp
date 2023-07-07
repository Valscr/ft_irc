/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vescaffr <vescaffr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 13:16:44 by valentin          #+#    #+#             */
/*   Updated: 2023/07/07 17:02:24 by vescaffr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

#include <iostream>
#include <vector>
#include <list>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cerrno>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

class User
{
    private:
        std::string _nickname[MAX_CLIENTS + 1];
        std::string _username[MAX_CLIENTS + 1];
    public:
        User();
        ~User() {};
        int setNickname(std::string nickname, int i);
        void setUsername(std::string username, int i);
        std::string returnNickname(int i);
        std::string returnUsername(int i);
};

std::string find_next_word(int i, std::string str);

std::string to_string(int value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

User::User()
{
    for (int i = 0; i <= MAX_CLIENTS; i++)
    {
        this->_nickname[i] = ("Client " + to_string(i)).c_str();
        setUsername(("Client " + to_string(i)).c_str(), i);
    }
}

int User::setNickname(std::string nickname, int i)
{
    if (!nickname.empty())
    {
        for (int j = 0; j <= MAX_CLIENTS; j++)
        {
            if (nickname == this->_nickname[j] && j != i)
            {
                this->_nickname[i] = (nickname + "_").c_str();
                return (1);
            }
        }
        this->_nickname[i] = nickname;
        return (0);
    }
    return (1);
}

void User::setUsername(std::string username, int i)
{
    if (!username.empty())
        this->_username[i] = username;
}

std::string User::returnNickname(int i)
{
    return (this->_nickname[i]);
}

std::string User::returnUsername(int i)
{
    return (this->_username[i]);
}

#endif