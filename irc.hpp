/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 13:16:44 by valentin          #+#    #+#             */
/*   Updated: 2023/07/06 15:12:21 by valentin         ###   ########.fr       */
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

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

class User
{
    private:
        std::string _nickname[MAX_CLIENTS];
        std::string _username[MAX_CLIENTS];
    public:
        User();
        ~User() {};
        void setNickname(std::string nickname, int i);
        void setUsername(std::string username, int i);
        void printUsername(int i);
        void printNickname(int i);
};

User::User()
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        setNickname("Client " + std::to_string(i), i);
        setUsername("Client " + std::to_string(i), i);
    }
}
void User::setNickname(std::string nickname, int i)
{
    if (!nickname.empty())
        this->_nickname[i] = nickname;
}

void User::setUsername(std::string username, int i)
{
    if (!username.empty())
        this->_username[i] = username;
}

void User::printUsername(int i)
{
    std::cout << this->_username[i];
}

void User::printNickname(int i)
{
    std::cout << this->_nickname[i];
}

#endif