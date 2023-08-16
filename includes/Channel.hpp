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
#include <set>

class Server;
class Channel
{
    private:

        int                 _id;
        std::string         _name;
        bool                _havePassword;
        std::string         _password;
        bool                _hasLimit;
        bool                _topic_restriction;
        int                 _limit;
        std::vector<int>    _users;
        std::vector<int>    _operators;
        bool                _invite_mode;
        std::string         _topic;

    public:

        Channel(std::string name, int fd, std::string password, int i);
        Channel(){ return ;};
        ~Channel();
        void addOperator(int fd);
        void setInviteMode(bool state);
        void setPassMode(bool state);
        void setPassword(std::string newpass);
        void setTopic(std::string topic);
        void setTopicRestriction(bool state);
        void setlimitMode(bool state);
        void setLimit(int limit);
        int find_channels(int fd);
        int alreadyExist(int fd);
        void addUser(int fd);
        int is_operator(int fd);
        bool isInvited(int fd);
        std::vector<int>& getWhiteList() {return (this->_users);};
        std::vector<int>& getOperators() {return (this->_operators);};
        bool getHavePassword();
        int getTopicMode();
        std::string getPassword();
        std::string getName();
        bool getHasLimit();
        int getLimit();
        int getID(){ return _id;};
        bool getInviteMode();
        std::string getTopic();
        std::string getListUsers(Server server);
        void removeOperator(int fd);
        void removeWhiteList(int fd);
};

#endif