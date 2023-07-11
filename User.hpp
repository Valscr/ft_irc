/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:42:13 by valentin          #+#    #+#             */
/*   Updated: 2023/07/10 17:04:35 by valentin         ###   ########.fr       */
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
        int fd;
    public:
        User(std::string nickname, int i);
        ~User() {this->_nickname.clear(); this->fd = -1; this->_username.clear(); this->_realname.clear(); this->_hostname.clear();};
        std::string returnNickname();
        std::string returnUsername();
        std::string returnHostname();
        std::string returnRealname();
        int returnFd();
        void setUsername(std::string newUsername);
        void setHotsname(std::string newHostname);
        void setRealsname(std::string newRealname);
        
};

#endif