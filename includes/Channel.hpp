/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhali <skhali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:48:10 by valentin          #+#    #+#             */
/*   Updated: 2023/07/24 23:37:34 by skhali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <vector>
#include <algorithm>
#include "irc.hpp"

class Channel
{
    private:

        std::string         _name;
        std::vector<User*>   _users;
        std::vector<int>    _white_list;
        std::vector<int>    _operators;
        std::vector<int>    _bans;
        bool                _invite_mode;

    public:

        Channel(std::string name, int fd);
        ~Channel();
        void addWhiteList(int fd);
        void addOperator(int fd);
        void addBan(int fd);
        void removeWhiteList(int fd);
        void removeOperator(int fd);
        void removeBan(int fd);
        void setInviteMode(bool state);
        std::string getName();
        int find_channels(int fd);
        int alreadyExist(User *user);
        std::vector<User*> getUsers();

        std::vector<int>& getWhiteList() {return (this->_white_list);};
        std::vector<int>& getOperators() {return (this->_operators);};
        std::vector<int>& getBans() {return (this->_bans);};
};

#endif