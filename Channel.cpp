#include "Channel.hpp"

Channel::Channel(std::string name, int fd) : _name(name)
{
	this->_white_list.push_back(fd);
	this->_operators.push_back(fd);
	this->_invite_mode = false;
}

Channel::~Channel() {}

void Channel::addWhiteList(int fd)
{
	std::vector<int>::iterator it = std::find(_white_list.begin(), _white_list.end(), fd);
	if (it == _white_list.end())
		this->_white_list.push_back(fd);
}

void Channel::addOperator(int fd)
{
	std::vector<int>::iterator it = std::find(_operators.begin(), _operators.end(), fd);
	if (it == _operators.end())
		this->_operators.push_back(fd);
}

void Channel::addBan(int fd)
{
	std::vector<int>::iterator it = std::find(_bans.begin(), _bans.end(), fd);
	if (it == _bans.end())
		this->_bans.push_back(fd);
}

void Channel::removeWhiteList(int fd)
{
	std::vector<int>::iterator it = std::find(_white_list.begin(), _white_list.end(), fd);
	if (it != _white_list.end())
		_white_list.erase(it);
}

void Channel::removeOperator(int fd)
{
	std::vector<int>::iterator it = std::find(_operators.begin(), _operators.end(), fd);
	if (it != _operators.end())
		_operators.erase(it);
}

void Channel::removeBan(int fd)
{
	std::vector<int>::iterator it = std::find(_bans.begin(), _bans.end(), fd);
	if (it != _bans.end())
		_bans.erase(it);
}

void Channel::setInviteMode(bool state)
{
	this->_invite_mode = state;
}