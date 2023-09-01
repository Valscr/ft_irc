#include "../includes/Channel.hpp"

Channel::Channel(std::string name, int fd, std::string password, int i) : _name(name)
{
	//this->_operators.push_back(fd);
	(void)fd;
	this->_invite_mode = false;
	this->_hasLimit = false;
	if (password == "")
		this->_havePassword = false;
	else
	{
		this->_havePassword = true;
		this->_password = password;
	}
	this->_id = i;
}

Channel::~Channel() {}


void Channel::addOperator(int fd)
{
	std::vector<int>::iterator it = std::find(this->_operators.begin(), this->_operators.end(), fd);
	if (it == this->_operators.end())
		this->_operators.push_back(fd);
}


void Channel::addUser(int fd)
{
	this->_users.push_back(fd);
}

int Channel::find_channels(int fd)
{
	std::vector<int>::iterator it = std::find(this->_users.begin(), this->_users.end(), fd);
	if (it != this->_users.end())
		return (1);
	std::vector<int>::iterator ito = std::find(this->_operators.begin(), this->_operators.end(), fd);
	if (ito != this->_operators.end())
		return (1);
	return (0);
}

bool Channel::isInvited(int fd)
{
	std::vector<int>::iterator it = std::find(this->_users.begin(), this->_users.end(), fd);
	if (it != this->_users.end())
		return (true);
	return (false);
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
	std::vector<int>::iterator it = std::find(this->_users.begin(), this->_users.end(), fd);
	if (it != this->_users.end())
		this->_users.erase(it);
}

void Channel::removeOperator(int fd)
{
	std::vector<int>::iterator it = std::find(this->_operators.begin(), this->_operators.end(), fd);
	if (it != this->_operators.end())
		this->_operators.erase(it);
}

void Channel::setInviteMode(bool state)
{
	this->_invite_mode = state;
}

void Channel::setTopicRestriction(bool state)
{
	this->_topic_restriction = state;
}

void Channel::setPassMode(bool state)
{
	this->_havePassword = state;
}

void Channel::setPassword(std::string newpass)
{
	this->_password = newpass;
}

void Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

void Channel::setLimit(int limit)
{
	this->_limit = limit;
}

void Channel::setlimitMode(bool state)
{
	this->_hasLimit = state;
}

std::string Channel::getName()
{
    return (this->_name);
}

int Channel::alreadyExist(int fd)
{
	if(this->_users.empty())
		return (0);
	for (std::vector<int>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
		if (*it == fd)
			return 1;
    }
	for (std::vector<int>::iterator ot = this->_operators.begin(); ot != this->_operators.end(); ++ot) {
		if (*ot == fd)
			return 1;
    }
	/*for (size_t i = 0; i < this->_users.size(); i++)
	{
		if (this->_users[i] == fd)
			return (1);
	}*/
	return (0);
}

bool Channel::getHavePassword()
{
	return (this->_havePassword);
}

int Channel::getTopicMode()
{
	return (this->_topic_restriction);
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

std::string Channel::getTopic()
{
	return (this->_topic);
}
template< typename T >
bool isInVector(std::vector<T> list, T element) 
{
	int found = false;
	typename std::vector<T>::iterator vict;
    for (vict = list.begin(); vict != list.end(); ++vict)
    {
        if (*vict == element)
        {
            found = true;
            break;
        }
    }
	return (found);
}

std::string Channel::getListUsers(Server server)
{
	std::string txt;
	for (std::vector<int>::iterator it = this->_operators.begin(); it != this->_operators.end(); ++it) {
		if (isInVector(this->_operators, *it))
			txt += "@";
		txt += server.getUser(*it).returnNickname();
		txt += " ";
    }
	for (std::vector<int>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
		if (!isInVector(this->_operators, *it))
		{
			txt += server.getUser(*it).returnNickname();
			txt += " ";
		}
	}
	
	/*for (size_t i = 0; i < this->_users.size(); i++)
	{
		if (isInVector(this->_operators, this->_users[i]))
			txt += "@";
		txt += server.getUser(this->_users[i]).returnNickname();
		txt += " ";
	}*/
	std::cout << "liste de users " << txt << std::endl;
	return (txt);
}
