/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smecili <smecili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:42:42 by valentin          #+#    #+#             */
/*   Updated: 2023/07/20 17:46:25 by smecili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/User.hpp"
#include "../includes/irc.hpp"

std::string to_string(int value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

User::User(std::string nickname, int fd, int id)
{
    if (nickname == "")
        this->_nickname = ("Client" + to_string(fd)).c_str();
    else
        this->_nickname = nickname;
    this->_username = ("Client" + to_string(fd)).c_str();
    this->_fd = fd;
    this->_id = id;
    this->_password = false;
}

std::string User::returnNickname()
{
    return (this->_nickname);
}

bool User::returnPassword()
{
    return (this->_password);
}

void User::setPassword(bool value)
{
    this->_password = value;
}

std::string User::returnUsername()
{
    return (this->_username);
}

std::string User::returnHostname()
{
    return (this->_hostname);
}

std::string User::returnRealname()
{
    return (this->_realname);
}

int User::returnFd()
{
    return (this->_fd);
}

int User::returnId()
{
    return (this->_id);
}

void User::setUsername(std::string newUsername)
{
    this->_username = newUsername;
}

void User::setHotsname(std::string newHostname)
{
    this->_hostname = newHostname;
}

void User::setRealsname(std::string newRealname)
{

    this->_realname = newRealname;
    
}