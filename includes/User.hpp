/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyacini <kyacini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:42:13 by valentin          #+#    #+#             */
/*   Updated: 2023/07/30 21:36:49 by kyacini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include "irc.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cerrno>

class User
{
    private:

        std::string _nickname;
        std::string _username;
        std::string _hostname;
        std::string _realname;
        bool _password;
        bool _registered;
        bool _hasnickname;
        int _fd, _id;

    public:

        User(std::string nickname, int fd, int id);
        ~User() {this->_nickname.clear(); this->_fd = -1; this->_username.clear(); this->_realname.clear(); this->_hostname.clear();};

        /* Getters */
        std::string returnNickname();
        std::string returnUsername();
        std::string returnHostname();
        std::string returnRealname();
        int returnFd();
        int returnId();
        bool returnPassword();
        bool returnRegistered();
        bool returnHasNickname();
    
        /* Setters */
        void setNickname(std::string newNickname);
        void setUsername(std::string newUsername);
        void setHotsname(std::string newHostname);
        void setRealsname(std::string newRealname);
        void setPassword(bool value);
        void setRegistered(bool value);
        void setHasNickname(bool value);

        
};

#endif