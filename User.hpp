/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:42:13 by valentin          #+#    #+#             */
/*   Updated: 2023/07/07 22:28:02 by valentin         ###   ########.fr       */
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
        std::string _nickname[MAX_CLIENTS + 1];
        std::string _username[MAX_CLIENTS + 1];
    public:
        User();
        ~User() {};
        int setNickname(std::string nickname, int i);
        void setUsername(std::string username, int i);
        std::string returnNickname(int i);
        std::string returnUsername(int i);
};

#endif