/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:48:10 by valentin          #+#    #+#             */
/*   Updated: 2023/07/27 21:55:40 by valentin         ###   ########.fr       */
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
        std::string         _topic;
        std::vector<int>    _white_list;
        std::vector<int>    _operators;
        std::vector<int>    _bans;
        bool                _topic_restriction;
        bool                _invite_mode;
    public:
        Channel();
        Channel(std::string name, int fd);
        ~Channel();
        void addWhiteList(int fd);
        void addOperator(int fd);
        void addBan(int fd);
        void removeWhiteList(int fd);
        void removeOperator(int fd);
        void removeBan(int fd);
        void setInviteMode(bool state);
        void setTopic(std::string topic);
        std::string getName();
        int find_user_channels(int fd);
        int is_ban(int fd);
        int is_operator(int fd);
        int getInviteMode();
        void setTopicRestriction(bool state);
        int getTopicMode();

        std::vector<int>& getWhiteList() {return (this->_white_list);};
        std::vector<int>& getOperators() {return (this->_operators);};
        std::vector<int>& getBans() {return (this->_bans);};
};

#endif