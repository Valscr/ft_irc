/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:42:13 by valentin          #+#    #+#             */
/*   Updated: 2023/07/08 02:17:40 by valentin         ###   ########.fr       */
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

#define MAX_CLIENTS 10

class User
{
    private:
        std::string _nickname;
        std::string _username;
        //int fd;
    public:
        User(std::string nickname, std::string username, int i);
        ~User() {};
        std::string returnNickname();
        std::string returnUsername();
};

#endif