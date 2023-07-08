/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 13:17:02 by valentin          #+#    #+#             */
/*   Updated: 2023/07/08 02:23:10 by valentin         ###   ########.fr       */
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
    std::vector<std::string> send_client(0);

    while (true)
    {
        if (poll(server.get_fds().data(), server.get_fds().size(), -1) < 0)
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
                        server.add_bool_pass();
                        server.add_bool_welcome();
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
                        std::cout << server.getUser()[i].returnNickname() << " déconnecté" << std::endl;
                        server.deleteUser(i);
                        server.delete_password_bool(i);
                        server.delete_welcome_bool(i);
                    } 
                    else if (server.get_bool_pass()[i] == false)
                    {
                        if (server.get_password() == find_next_word(buffer.find("PASS") + 5, buffer))
                            server.password_true(i);
                        else
                            send(server.get_fds()[i].fd, ":server-irc 464 client :Mot de passe requis\n", 45, 0);
                    }
                    if (buffer.find("NICK") != std::string::npos)
                    {
                        server.createUser(find_next_word(buffer.find("NICK") + 5, buffer), find_next_word(buffer.find("USER") + 5, buffer), i);
                        
                        if (server.get_bool_pass()[i] == true)
                        {
                            send(server.get_fds()[i].fd, (":server-irc 001 " + server.getUser()[i].returnNickname() + " :Bienvenue sur le serveur IRC " + server.getUser()[i].returnNickname() + "\n").c_str(), 17 + 32 + 2 + (server.getUser()[i].returnNickname().length() * 2), 0);
                            server.welcome_true(i);
                        }
                    }
                    if (server.get_bool_welcome()[i] == true)
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
