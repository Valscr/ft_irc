/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_exec.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 11:02:33 by valentin          #+#    #+#             */
/*   Updated: 2023/07/09 23:17:34 by valentin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

bool run;

void handleSignal(int signal)
{
    (void)signal;
    if (run == true)
    {
        run = false;
        std::cout << std::endl <<  ANSI_YELLOW << "server closing." << ANSI_RESET << std::endl;
    }
}

void server_exec(Server &server)
{
    bool password[MAX_CLIENTS + 1];
    bool welcome[MAX_CLIENTS + 1];

    run = true;
    for (int j = 0; j <= MAX_CLIENTS; j++)
    {
        password[j] = false;
        welcome[j] = false;
    }
    
    signal(SIGINT, handleSignal);
    while (run == true)
    {
        if (poll(server.get_fds().data(), server.get_fds().size(), -1) < 0)
            handleSignal(1);
        // Vérifier les événements sur les descripteurs de fichiers surveillés
        for (size_t i = 0; i < server.get_fds().size(); i++)
        {
            if (server.get_fds()[i].revents)
            {
                if (server.get_fds()[i].fd == server.getListensocket())
                {
                    // Événement sur le socket d'écoute (nouvelle connexion entrante)
                    server.set_Clientsocket(accept(server.getListensocket(), NULL, NULL)); 
                    if (server.getClientsocket() < 0) {
                        if (errno != EWOULDBLOCK && errno != EAGAIN)
                            throw std::runtime_error("not accepted connexion");
                    } else {
                        // Mettre le socket client en mode non-bloquant
                        if (fcntl(server.getClientsocket(), F_SETFL, O_NONBLOCK) < 0)
                            throw std::runtime_error("configuration socket in non-blocking mode");
                        server.set_fds_i_fd(i);
                    }
                }
                else 
                {
                    // Événement sur un socket client existant (données reçues)
                    std::string buffer(BUFFER_SIZE, '\0');
                    int bytesRead = recv(server.get_fds()[i].fd, &buffer[0], buffer.size(), 0);
                    if (run == true)
                        std::cout << ANSI_BLUE << "< " << buffer << ANSI_RESET << std::endl;
                    if (bytesRead < 0)
                    {
                        if (errno != EWOULDBLOCK && run == true && errno != EAGAIN)
                            throw std::runtime_error("reading buffer");
                    }
                    else if (bytesRead == 0)
                    {
                        // Connexion fermée par le client
                        server.close_fd(i);
                        std::cout << server.getUser(i).returnNickname() << " disconnected" << std::endl;
                        server.deleteUser(i);
                    } 
                    else if (password[i] == false)
                    {
                        if (server.get_password() == find_next_word(buffer.find("PASS") + 5, buffer))
                            password[i] = true;
                        else
                        {
                            send(server.get_fds()[i].fd, msg_464().c_str(), msg_464().length(), 0);
                            std::cout << "> " << msg_464().c_str() << std::endl;
                        }
                    }
                    if (buffer.find("NICK") != std::string::npos)
                    {
                        if (password[i] == true)
                        {
                            server.createUser(find_next_word(buffer.find("NICK") + 5, buffer), i);
                            server.get_send().insert(std::make_pair(i, ""));
                            send(server.get_fds()[i].fd, msg_001(server.getUser(i).returnNickname()).c_str(), msg_001(server.getUser(i).returnNickname()).length(), 0);
                            std::cout << ANSI_GREEN << "> " << msg_001(server.getUser(i).returnNickname()).c_str() << ANSI_RESET << std::endl;
                            welcome[i] = true;
                        }
                    }
                    if (buffer.find("USER") != std::string::npos)
                    {
                        if (server.UserExist_fd(i))
                            server.getUser(i).setUsername(find_next_word(buffer.find("USER") + 5, buffer));
                    }
                   
                    if (welcome[i] == true)
                    {
                        parse_buffer(buffer, server, i);
                        send_function(server, server.get_fds());
                    }
                }
            }
        }
    }
    for (size_t i = 0; i < server.get_fds().size(); i++)
        close(server.get_fds()[i].fd);
}