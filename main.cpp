/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 13:17:02 by valentin          #+#    #+#             */
/*   Updated: 2023/07/07 23:06:04 by valentin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Server.hpp"
#include "User.hpp"

std::string find_next_word(int i, std::string str)
{
    int j = i;
    while (str[i] != '\r' && str[i] != '\0' && str[i] != ' ' && str[i] != '\n')
        i++;
    return (str.substr(j, i - j));
}

std::string parse_buffer(std::string buffer)
{
    if (buffer.find("PING") != std::string::npos)
        return ("PONG server-irc\n");
    return ("");
}

void send_function(std::vector<std::string> &send_client, int i, std::vector<pollfd> fds)
{
    std::vector<std::string>::const_iterator it;
    for (it = send_client.begin(); it != send_client.end(); ++it)
    {
        const std::string& str = *it;
        const char* data = str.c_str(); 
        send(fds[i].fd, data, std::strlen(data), 0);
    }
    send_client.clear();
}

int main(int argc, char **argv)
{
    if (argc != 3)
        return (0);
    Server server(std::atoi(argv[1]), argv[2]);
    int nickname[MAX_CLIENTS + 1];
    int password[MAX_CLIENTS + 1];
    int welcome[MAX_CLIENTS + 1];
    std::vector<std::string> send_client(0);
    User user;

    while (true)
    {
        int result = poll(server.get_fds().data(), server.get_fds().size(), -1); // Attendre indéfiniment
        if (result < 0)
        {
            perror("Erreur lors de l'appel à poll");
            exit(1);
        }
        // Vérifier les événements sur les descripteurs de fichiers surveillés
        for (size_t i = 0; i <= server.get_fds().size(); i++)
        {
            if (server.get_fds()[i].revents)
            {
                if (server.get_fds()[i].fd == server.getListensocket())
                {
                    // Événement sur le socket d'écoute (nouvelle connexion entrante)
                    server.set_Clientsocket(accept(server.getListensocket(), NULL, NULL)); 
                    if (server.getClientsocket() < 0) {
                        if (errno != EWOULDBLOCK && errno != EAGAIN) {
                            perror("Erreur lors de l'acceptation de la connexion");
                            exit(1);
                        }
                    } else {
                        // Mettre le socket client en mode non-bloquant
                        if (fcntl(server.getClientsocket(), F_SETFL, O_NONBLOCK) < 0) {
                            perror("Erreur lors de la configuration du socket client en mode non-bloquant");
                            exit(1);
                        }
                        server.set_fds_i_fd(i);
                    }
                }
                else 
                {
                    // Événement sur un socket client existant (données reçues)
                    std::string buffer(BUFFER_SIZE, '\0');

                    int bytesRead = recv(server.get_fds()[i].fd, &buffer[0], buffer.size(), 0);
                    std::cout << buffer << std::endl;
                    if (bytesRead < 0)
                    {
                        if (errno != EWOULDBLOCK && errno != EAGAIN) {
                            perror("Erreur lors de la lecture des données");
                            exit(1);
                        }
                    }
                    else if (bytesRead == 0)
                    {
                        // Connexion fermée par le client
                        server.close_fd(i);
                        std::cout << user.returnNickname(i) << " déconnecté" << std::endl;
                        user.setUsername(("Client " + std::to_string(i)).c_str(), i);
                        user.setNickname(("Client " + std::to_string(i)).c_str(), i);
                    } 
                    else if (password[i] == 0)
                    {
                        if (server.get_password() == find_next_word(buffer.find("PASS") + 5, buffer))
                        {
                            password[i] = 1;
                        }
                        else
                        {
                            send(server.get_fds()[i].fd, ":server-irc 464 client :Mot de passe requis\n", 45, 0);
                        }
                    }
                    if (welcome[i] == 0 && buffer.find("NICK") != std::string::npos)
                    {
                        nickname[i] = user.setNickname(find_next_word(buffer.find("NICK") + 5, buffer), i);
                        user.setUsername(find_next_word(buffer.find("USER") + 5, buffer), i);
                        std::string nick = find_next_word(buffer.find("NICK") + 5, buffer);
                        while (nickname[i] == 1)
                        {
                            nickname[i] = user.setNickname(user.returnNickname(i), i);
                            send_client.push_back((":server-irc 433 * " + nick + " :Nickname is already in use\n").c_str());
                            nick = (nick + "_").c_str();
                        }
                        send_function(send_client, i, server.get_fds());
                        if (password[i] == 1)
                        {
                            send(server.get_fds()[i].fd, (":server-irc 001 " + user.returnNickname(i) + " :Bienvenue sur le serveur IRC " + user.returnNickname(i) + "\n").c_str(), 17 + 32 + 2 + user.returnNickname(i).length() + user.returnNickname(i).length(), 0);
                            welcome[i] = 1;
                        }
                        std::cout << "USER = " << user.returnUsername(i) << std::endl;
                    }
                    if (welcome[i] == 1)
                    {
                        send_client.push_back(parse_buffer(buffer));
                        send_function(send_client, i, server.get_fds());
                    }
                }
            }
        }
    }
    return 0;
}
