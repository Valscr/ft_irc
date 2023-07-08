/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 20:48:10 by valentin          #+#    #+#             */
/*   Updated: 2023/07/08 19:58:53 by valentin         ###   ########.fr       */
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
        std::vector<int>    _white_list;
        std::vector<int>    _operators;
        std::vector<int>    _bans;
        bool                _invite_mode;
    public:
        Channel(std::string name, int fd);
        // la copie et la surcharge de '=' ne me semblent pas utiles 
        ~Channel();
        void addWhiteList(int fd);
        void addOperator(int fd);
        void addBan(int fd);
        void removeWhiteList(int fd);
        void removeOperator(int fd);
        void removeBan(int fd);
        void setInviteMode(bool state);
        std::string getName();

        const std::vector<int>& getWhiteList() const;
        const std::vector<int>& getOperators() const;
        const std::vector<int>& getBans() const;
};

#endif