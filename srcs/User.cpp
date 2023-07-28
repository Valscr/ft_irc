/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyacini <kyacini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:42:42 by valentin          #+#    #+#             */
/*   Updated: 2023/07/28 17:19:36 by kyacini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/User.hpp"
#include "../includes/irc.hpp"

/***********************************************************************/
/*                                UTILS                               */
/*********************************************************************/
std::string to_string(int value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

/***********************************************************************/
/*                   CONSTRUCTOR & DESTRUCTOR                         */
/*********************************************************************/

User::User(std::string nickname, int fd, int id)
{
    if (nickname == "")
        this->_nickname = ("Client" + to_string(fd)).c_str();
    else
        this->_nickname = nickname;
    this->_fd = fd;
    this->_id = id;
    this->_password = false;
    this->_registered = false;
    this->_hasnickname = false;
}

/***********************************************************************/
/*                          GETTERS & SETTERS                         */
/*********************************************************************/

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

bool User::returnRegistered()
{
    return (this->_registered);
}

bool User::returnHasNickname()
{
    return (this->_hasnickname);
}
void User::setRegistered(bool value)
{
    this->_registered = value;
}

void User::setHasNickname(bool value)
{
    this->_hasnickname = value;
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

void User::setNickname(std::string newNickname)
{

    this->_nickname = newNickname;  
}
