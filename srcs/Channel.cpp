/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyacini <kyacini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:48:05 by valentin          #+#    #+#             */
/*   Updated: 2023/07/28 04:15:58 by kyacini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel(std::string name, int fd, std::string password) : _name(name)
{
	this->_operators.push_back(fd);
	this->_invite_mode = false;
	if (password == "")
		this->_havePassword = false;
	else
	{
		this->_havePassword = true;
		this->_password = password;
	}
}

Channel::~Channel() {}

void Channel::addWhiteList(int fd)
{
	std::vector<int>::iterator it = std::find(this->_white_list.begin(), this->_white_list.end(), fd);
	if (it == this->_white_list.end())
		this->_white_list.push_back(fd);
}

void Channel::addOperator(int fd)
{
	std::vector<int>::iterator it = std::find(this->_operators.begin(), this->_operators.end(), fd);
	if (it == this->_operators.end())
		this->_operators.push_back(fd);
}

void Channel::addBan(int fd)
{
	std::vector<int>::iterator it = std::find(this->_bans.begin(), this->_bans.end(), fd);
	if (it == this->_bans.end())
		this->_bans.push_back(fd);
}

void Channel::addUser(User *user)
{
	this->_users.push_back(user);
}

int Channel::find_channels(int fd)
{
	std::vector<int>::iterator it = std::find(this->_white_list.begin(), this->_white_list.end(), fd);
	if (it != this->_white_list.end())
		return (1);
	std::vector<int>::iterator ito = std::find(this->_operators.begin(), this->_operators.end(), fd);
	if (ito != this->_operators.end())
		return (1);
	return (0);
}

bool Channel::isInvited(int fd)
{
	std::vector<int>::iterator it = std::find(this->_white_list.begin(), this->_white_list.end(), fd);
	if (it != this->_white_list.end())
		return (true);
	return (false);
}

void Channel::removeWhiteList(int fd)
{
	std::vector<int>::iterator it = std::find(this->_white_list.begin(), this->_white_list.end(), fd);
	if (it != this->_white_list.end())
		this->_white_list.erase(it);
}

void Channel::removeOperator(int fd)
{
	std::vector<int>::iterator it = std::find(this->_operators.begin(), this->_operators.end(), fd);
	if (it != this->_operators.end())
		this->_operators.erase(it);
}

void Channel::removeBan(int fd)
{
	std::vector<int>::iterator it = std::find(this->_bans.begin(), this->_bans.end(), fd);
	if (it != this->_bans.end())
		this->_bans.erase(it);
}

void Channel::setInviteMode(bool state)
{
	this->_invite_mode = state;
}

std::vector<User*> Channel::getUsers()
{
	return (this->_users);
}

std::string Channel::getName()
{
    return (this->_name);
}

int Channel::alreadyExist(User *user)
{
	if(this->_users.empty())
		return (0);
	for (size_t i = 0; i < this->_users.size(); i++)
	{
		if (this->_users[i] == user)
			return (1);
	}
	return (1);
}

bool Channel::getHavePassword()
{
	return (this->_havePassword);
}

std::string Channel::getPassword()
{
	return (this->_password);
}

bool Channel::getHasLimit()
{
	return (this->_hasLimit);
}

int Channel::getLimit()
{
	return (this->_limit);
}

bool Channel::getInviteMode()
{
	return (this->_invite_mode);
}