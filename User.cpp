/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:42:42 by valentin          #+#    #+#             */
/*   Updated: 2023/07/08 12:05:56 by valentin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "irc.hpp"

std::string to_string(int value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

User::User(std::string nickname, std::string username, int i)
{
    if (nickname.empty())
        this->_nickname = ("Client" + to_string(i)).c_str();
    else
        this->_nickname = nickname;
    if (username.empty())
        this->_username = ("Client" + to_string(i)).c_str();
    else
        this->_username = username;
    this->fd = i;
}

std::string User::returnNickname()
{
    return (this->_nickname);
}

std::string User::returnUsername()
{
    return (this->_username);
}

int User::returnFd()
{
    return (this->fd);
}
