/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_exec.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhali <skhali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 11:02:33 by valentin          #+#    #+#             */
/*   Updated: 2023/07/23 16:31:12 by skhali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"
#include "../includes/Commands.hpp"
typedef void (*fct)(std::vector<std::string> &, int, Server &);

bool run;
int disconnect(int i, Server &server, bool end)
{
    int id;
    try {
        id = server.getUser(server.get_fds()[i].fd).returnId();
        server.deleteUser(server.get_fds()[i].fd);
        server.close_fd(i);
        server.erase_fd(i);
    } catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what();
        return (server.freeEverything(), 0);
    }
    if (!end)
        std::cout << "User " << id << " disconnected"<< std::endl;
    return (1);
}

void handleSignal(int signal)
{
    (void)signal;
    if (run == true)
    {
        run = false;
        std::cout << std::endl <<  ANSI_YELLOW << "server closing." << ANSI_RESET << std::endl;
    }
}

void new_user(Server &server)
{
    static int id = 1;
    int nb_fds = server.get_fds().size();
    server.set_Clientsocket(accept(server.getListensocket(), NULL, NULL)); 
    if (server.getClientsocket() < 0) {
        if (errno != EWOULDBLOCK && errno != EAGAIN)
            throw std::runtime_error("not accepted connexion");
    } else {
        if (fcntl(server.getClientsocket(), F_SETFL, O_NONBLOCK) < 0)
            throw std::runtime_error("configuration socket in non-blocking mode");
        server.set_fds_i_fd();
        server.createUser("", id, server.get_fds().back().fd);
        std::cout << nb_fds << std::endl;
        int fd = server.getUsersList()[nb_fds - 1].returnFd();
		std::cout << "Client " << id << "[" << fd << "]" << " connected" << std::endl;
        id++;
    }
}
std::vector<std::string>	split_buffer(int fd, std::string str, Server *server) {
	std::string			line;
	std::vector<std::string>	res;

    server->add_send(fd, str);

    std::string input;
    std::istringstream r_iss(server->get_send()[fd]);
    while (std::getline(r_iss, input, '\r')) {
        line.append(input);
    }
    std::istringstream n_iss(line);
    while (std::getline(n_iss, input, '\n')) {
        res.push_back(input);
    }
	return res;
}
std::vector<std::string>	split_command(std::string str) {
	std::vector<std::string>	res;

    std::string input;
    std::istringstream r_iss(str);
    while (std::getline(r_iss, input, ' ')) {
        res.push_back(input);
    }
	return res;
}

void exec_command(std::vector<std::string> command, Server &server, int id, std::map<std::string, Commands::fct> services)
{
    std::map<std::string, Commands::fct>::iterator it = services.find(command[0]);

    if (it != services.end()) {
        Commands cmdObject; // Créez une instance de la classe Commands
        Commands::fct cmd = it->second;
        (cmdObject.*cmd)(command, id, server); // 
    } else {
        msg_421();
    }

}

int server_exec(Server &server)
{
    char buffer[BUFFER_SIZE];
    //bool password[MAX_CLIENTS + 1];
    //bool welcome[MAX_CLIENTS + 1];
    bool run = true;
    /*for (int j = 0; j <= MAX_CLIENTS; j++)
    {
        //password[j] = false;
        welcome[j] = false;
    }*/
    
    signal(SIGINT, handleSignal);
    while (run)
    {
        if (poll(server.get_fds().data(), server.get_fds().size(), -1) < 0)
            handleSignal(1);
        std::vector<pollfd> fds_test = server.get_fds();
        std::cout << "-------------- fd tableau ----------------------" << std::endl;
        for (std::vector<pollfd>::iterator it = fds_test.begin() ; it != fds_test.end(); ++it)
            std::cout << it->fd << std::endl;
        std::cout << "----------------" << server.get_fds().size() << "------------------------" << std::endl;
        std::vector<User> user_test = server.getUsersList();
        std::cout << "-------------- users tableau ----------------------" << std::endl;
        for (std::vector<User>::iterator it1 = user_test.begin() ; it1 != user_test.end(); ++it1)
            std::cout << it1->returnId() << " et fd : " << it1->returnFd()<< std::endl;
        std::cout << "----------------------------------------" << std::endl;
        // Vérifier les événements sur les descripteurs de fichiers surveillés
        for (size_t i = 0; i < server.get_fds().size(); i++)
        {
            //std::cout << "fd actuel :" << server.get_fds()[i].fd  << std::endl;
            if (server.get_fds()[i].revents)
            {
                if (server.get_fds()[i].fd == server.getListensocket())
                    new_user(server);
                else if (server.get_fds()[i].revents & POLLRDHUP)
                {
                    if(!disconnect(i, server, false))
                        return 0;
                }
                else 
                {
                    int bytesRead = recv(server.get_fds()[i].fd, buffer, BUFFER_SIZE, 0);
                    if (bytesRead < 0)
                    {
                        if (errno != EWOULDBLOCK && run == true && errno != EAGAIN)
                            throw std::runtime_error("reading buffer");
                    }
                    else if (bytesRead == 0)
                    {
                        if(!disconnect(i, server, false))
                            return 0;
                    }
                    /*else
                    {
                        buffer[bytesRead] = '\0';
                        std::string received_data(buffer);
                        //boucle pour etre sur d'avoir tout recu
                        std::cout << "------------ buffer de " << server.get_fds()[i].fd << "------------" << std::endl;
                        std::vector<std::string> inputs = split_buffer(server.get_fds()[i].fd, received_data, &server);
                        for (std::vector<std::string>::iterator it = inputs.begin() ; it != inputs.end(); ++it)
                        {
                            std::vector<std::string> command = split_command(*it);
                            //note : dans user, i = id + 1
                            //server.get_fds()[i].fd
                            exec_command(command, server, i, server.getCommand()->getServices());
                        }
                        std::cout << "----------------------------------------------" << std::endl;

                    }*/
                    /*else if (password[i] == false)
                    {
                        std::cout << "je teste si le mdp est correct" << server.get_password() << std::endl;
                         std::cout << find_next_word(buffer.find("PASS") + 5, buffer) << std::endl;
                        if (server.get_password() == find_next_word(buffer.find("PASS") + 5, buffer))
                        {
                            password[i] = true;
                        }
                        else
                        {
                            send(server.get_fds()[i].fd, msg_464().c_str(), msg_464().length(), 0);
                            std::cout << "> " << msg_464().c_str() << std::endl;
                        }
                    }*/
                    //else if (server.UserExist_fd(i))

                    /*if (buffer.find("NICK") != std::string::npos && password[i] == true)
                    {
                        if (password[i] == true)
                        {
                            server.createUser(find_next_word(buffer.find("NICK") + 5, buffer), i);
                            server.get_send().insert(std::make_pair(i, ""));
                            send(server.get_fds()[i].fd, msg_001(server.getUser(i).returnNickname()).c_str(), msg_001(server.getUser(i).returnNickname()).length(), 0);
                            std::cout << ANSI_GREEN << MAX_CLIENTS + 1 - i << " > " << msg_001(server.getUser(i).returnNickname()).c_str() << ANSI_RESET << std::endl;
                            welcome[i] = true;
                        }
                    }
                    if (buffer.find("USER") != std::string::npos && password[i] == true)
                    {
                        if (server.UserExist_fd(i))
                        {
                            server.getUser(i).setUsername(find_next_word(buffer.find("USER") + 5, buffer));
                            server.getUser(i).setRealsname(buffer.substr(buffer.find(":") + 1, buffer.size() - buffer.find(":")));
                            server.getUser(i).setHotsname(find_previous_word(buffer.find(":") - 1, buffer));
                        }
                    }*/
                    //if (welcome[i] == true)
                    //{

                        //parse_buffer(buffer, server, i);
                        //send_function(server, server.get_fds());
                    //}
                }
            }
        }
    }
    server.freeEverything();
    return 1;
}