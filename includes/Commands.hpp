#ifndef COMMANDS_HPP
# define COMMANDS_HPP

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
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <signal.h>

class Server;
class	Commands {
	public:
		Commands(void);
		~Commands(void);

		/* Getters & setters*/
		typedef int (Commands::*fct)(std::vector<std::string> &, int, Server &);
		std::map<std::string, fct> setServices(void)
		{
			std::map<std::string, fct> commandMap;
			commandMap.insert(std::make_pair("PASS", &Commands::PASS));
			commandMap.insert(std::make_pair("NICK", &Commands::NICK));
			commandMap.insert(std::make_pair("USER", &Commands::USER));
			commandMap.insert(std::make_pair("PING", &Commands::PING));
			commandMap.insert(std::make_pair("KICK", &Commands::KICK));
			commandMap.insert(std::make_pair("JOIN", &Commands::JOIN));
			return (commandMap);
		}
		std::map<std::string, fct> getServices();

		/* Commands methods*/
		int		PASS(std::vector<std::string> &command, int id, Server &server);
		int		NICK(std::vector<std::string> &command, int id, Server &server);
		int		USER(std::vector<std::string> &command, int id, Server &server);
		int		PING(std::vector<std::string> &command, int id, Server &server);
		int		JOIN(std::vector<std::string> &command, int id, Server &server);
		//int		QUIT(std::vector<std::string> &command, int id, Server &server);
		int		KICK(std::vector<std::string> &command, int id, Server &server);

		/* Utils */
		int validNickname(std::string nickname);
		private:
			std::map<std::string, fct> services_list;

};


#endif