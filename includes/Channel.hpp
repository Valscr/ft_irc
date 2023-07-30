#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include "Server.hpp"
#include <vector>
#include <list>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <signal.h>
#include "irc.hpp"
class Channel
{
    private:

        std::string         _name;
        bool                _havePassword;
        std::string         _password;
        bool                _hasLimit;
        int                 _limit;
        std::vector<User*>  _users;
        std::vector<int>    _white_list;
        std::vector<int>    _operators;
        bool                _invite_mode;
        std::string         _topic;

    public:

        Channel(std::string name, int fd, std::string password);
        Channel(){ return ;};
        ~Channel();
        void addWhiteList(int fd);
        void addOperator(int fd);
        void removeWhiteList(int fd);
        void removeOperator(int fd);
        void setInviteMode(bool state);
        std::string getName();
        int find_channels(int fd);
        int alreadyExist(User *user);
        void addUser(User *user);
        std::vector<User*> getUsers();

        bool isInvited(int fd);
        std::vector<int>& getWhiteList() {return (this->_white_list);};
        std::vector<int>& getOperators() {return (this->_operators);};
        bool getHavePassword();
        std::string getPassword();
        bool getHasLimit();
        int getLimit();
        bool getInviteMode();
        std::string getTopic();
};

#endif