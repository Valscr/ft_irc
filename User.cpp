/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:42:42 by valentin          #+#    #+#             */
/*   Updated: 2023/07/07 22:22:41 by valentin         ###   ########.fr       */
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