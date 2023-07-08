/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 13:17:02 by valentin          #+#    #+#             */
/*   Updated: 2023/07/08 13:30:45 by valentin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Server.hpp"
#include "User.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
        return (0);
    Server server(std::atoi(argv[1]), argv[2]);
    bool password[MAX_CLIENTS + 1];
    bool welcome[MAX_CLIENTS + 1];

    for (int j = 0; j <= MAX_CLIENTS; j++)
    {
        password[j] = false;
        welcome[j] = false;
    }
    
    while (true)
    {
        if (poll(server.get_fds().data(), server.get_fds().size(), -1) < 0)
        {
            perror("Erreur lors de l'appel à poll");
            exit(1);
        }
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
                        std::cout << server.getUser(i).returnNickname() << " déconnecté" << std::endl;
                        server.deleteUser(i);
                    } 
                    else if (password[i] == false)
                    {
                        if (server.get_password() == find_next_word(buffer.find("PASS") + 5, buffer))
                            password[i] = true;
                        else
                            send(server.get_fds()[i].fd, ":server-irc 464 client :Mot de passe requis\n", 45, 0);
                    }
                    if (buffer.find("NICK") != std::string::npos)
                    {
                        if (password[i] == true)
                        {
                            server.createUser(find_next_word(buffer.find("NICK") + 5, buffer), find_next_word(buffer.find("USER") + 5, buffer), i);
                            send(server.get_fds()[i].fd, (":server-irc 001 " + server.getUser(i).returnNickname() + " :Bienvenue sur le serveur IRC " + server.getUser(i).returnNickname() + "\n").c_str(), 17 + 32 + 2 + server.getUser(i).returnNickname().length() + server.getUser(i).returnNickname().length(), 0);
                            welcome[i] = true;
                        }
                    }
                    if (welcome[i] == true)
                    {
                        server.get_send().push_back(parse_buffer(buffer, server));
                        send_function(server.get_send(), i, server.get_fds());
                    }
                }
            }
        }
    }
    return 0;
}
