
#ifndef SERVER_HPP
# define SERVER_HPP

#include "irc.hpp"
#include <iostream>
#include <vector>
#include <map>
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
#include "Commands.hpp"

class User;
class Channel;
class Commands;

class Server
{
    private:
        int port, listenSocket, clientSocket;
        std::string name;
        std::vector<pollfd> fds;
        
        
        std::string password;
        std::vector<Channel> channels;
        std::vector<User> users;
        struct sockaddr_in serverAddress;
        std::map<int, std::string> rcv_client;
        std::map<int, std::string> send_client;
        Commands *commands;
    
    
    public:
        
        Server(int port, std::string password);
        ~Server() {};


        /****************GETTERS***************/

        std::vector<User> getUsersList();
        Channel& getChannel(std::string name);
        std::vector<pollfd> &get_fds();
        int getListensocket();
        int getClientsocket();
        Commands* getCommand();
        std::string get_password();
        User& getUser(int i);
        std::string  &get_send_fd(int fd);
        std::map<int, std::string> &get_send();
        std::string get_name();
        User& getUserwithNickname(std::string name);
        void  addmsg_send(int fd, std::string str);
        void  addmsg_rcv(int fd, std::string str);
        void  send_all(std::string msg, Channel chan);
        /******************SETTERS****************/

        void set_Clientsocket(int clientsocket);
        void set_fds_i_fd();

        /************USERS MANAGEMENT METHODS************/

        void createUser(std::string nickname, int id, int fd);
        void deleteUser(int index);
        int UserExist(std::string name);
        int UserExist_fd(int fd);
        bool NicknameMatching(std::string nickname);

        /***************CHANNELS MANAGEMENT METHODS*****************/

        void createChannel(std::string name, int fd, std::string password);
        int find_channel(std::string name);

        /**************autre***************************************/
        void close_fd(int i);
        void erase_send(int fd);
        void erase_fd(int i);
        void freeEverything();
};

#endif