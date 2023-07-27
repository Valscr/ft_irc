/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:48:05 by valentin          #+#    #+#             */
/*   Updated: 2023/07/27 20:21:33 by valentin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel()
{}

Channel::Channel(std::string name, int fd) : _name(name)
{
	this->_operators.push_back(fd);
	this->_invite_mode = false;
}

Channel::~Channel() {}

void Channel::addWhiteList(int fd)
{
	std::vector<int>::iterator it = std::find(this->_white_list.begin(), this->_white_list.end(), fd);
	if (it == this->_white_list.end())
		this->_white_list.push_back(fd);
}

void Channel::setTopic(std::string topic)
{
	this->_topic = topic;
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

int Channel::find_user_channels(int fd)
{
	std::vector<int>::iterator it = std::find(this->_white_list.begin(), this->_white_list.end(), fd);
	if (it != this->_white_list.end())
		return (1);
	std::vector<int>::iterator ito = std::find(this->_operators.begin(), this->_operators.end(), fd);
	if (ito != this->_operators.end())
		return (1);
	return (0);
}

int Channel::is_ban(int fd)
{
	std::vector<int>::iterator it = std::find(this->_bans.begin(), this->_bans.end(), fd);
	if (it != this->_bans.end())
		return (1);
	return (0);
}

int Channel::is_operator(int fd)
{
	std::vector<int>::iterator it = std::find(this->_operators.begin(), this->_operators.end(), fd);
	if (it != this->_operators.end())
		return (1);
	return (0);
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

int Channel::getInviteMode()
{
	return (this->_invite_mode);
}

std::string Channel::getName()
{
    return (this->_name);
}