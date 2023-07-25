#pragma once

//Message PASS
# define ERR_NEEDMOREPARAMS(command) ("461 " + command + " :Not enough parameters\r\n")  //Message USER and OPER and KILL
# define ERR_ALREADYREGISTERED "462 :Unauthorized command (already registered)\r\n"      //Message SERVER and USER

//Message NICK
# define ERR_NONICKNAMEGIVEN "431 :No nickname given\r\n"
# define ERR_ERRONEUSNICKNAME(nick) ("432 " + nick + " :Erroneous nickname\r\n")
# define ERR_NICKNAMEISUSE(nick) ("433 NICK :" + nick + " :Nick already in use\r\n")
# define ERR_NICKCOLLISION(nick) ("436 NICK :" + nick + " :Nick collision\r\n")

//Message SQUIT
# define ERR_NOPRIVILEGES "481 :Permission Denied- You're not an IRC operator\r\n"       //Message KILL
# define ERR_NOSUCHSERVER (serv) ("402 SERVER :" + serv + " :Server does not exist\r\n") //Messages PING/PONG

//Messages PING/PONG
# define ERR_NOORIGIN "409 :Originator parameter is missing\r\n"

//Message OPER
# define RPL_YOUREOPER(nick) ("381 " + nick + " :You are now an IRC operator\r\n")
# define ERR_NOOPERHOST "491 :You cannot become an IRC operator :The client's host disallow it\r\n"
# define ERR_PASSWDMISMATCH "464 :Password incorrect\r\n"

//Message KILL
# define ERR_NOSUCHNICK(invitenick) ("401 " + invitenick + " :No such nick/channel\r\n")
# define ERR_CANTKILLSERVER "483 :Servers can't be killed\r\n"

//Message SERVER
# define RPL_WELCOME(nick, user, host) ("001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host + "\r\n")
# define RPL_NAMREPLY(nick, user, host, channel) ("353 " + nick + "!" + user + "@" + host + " = " + channel + " :")

//Message COMMAND
# define ERR_UNKNOWNCOMMAND(command) ("421 " + command + " :Unknown command\r\n")
