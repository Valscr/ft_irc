/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 13:17:02 by valentin          #+#    #+#             */
/*   Updated: 2023/07/07 00:07:08 by valentin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string find_next_word(int i, std::string str)
{
    int j = i;
    while ((std::isalpha(str[i]) && i < str.length()) || (str[i] != '\r' && str[i] != '\0' && str[i] != ' '))
        i++;
    return (str.substr(j, i - j));
}

void parse_buffer(std::string buffer, User user, int i)
{
    if (!buffer.find("NICK"))
    {
        user.setNickname(find_next_word(buffer.find("NICK") + 5, buffer), i);
    }
}

int main(int argc, char **argv)
{
    if (argc != 3)
        return (0);
    std::string pass = argv[2];
    int port = std::atoi(argv[1]);
    int nickname[MAX_CLIENTS + 1];
    int password[MAX_CLIENTS + 1];
    int welcome[MAX_CLIENTS + 1];
    int listenSocket, clientSocket;
    struct sockaddr_in serverAddress;
    std::vector<pollfd> fds(MAX_CLIENTS + 1); // +1 pour le socket d'écoute
    User user;
    // Création du socket d'écoute
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0)
    {
        perror("Erreur lors de la création du socket d'écoute");
        exit(1);
    }

    // Configuration de l'adresse du serveur
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port); // Port d'écoute

    // Lier le socket d'écoute à l'adresse et au port spécifiés
    if (bind(listenSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0)
    {
        perror("Erreur lors de la liaison du socket d'écoute");
        exit(1);
    }

    // Mettre le socket d'écoute en mode écoute
    if (listen(listenSocket, MAX_CLIENTS) < 0)
    {
        perror("Erreur lors de la mise en écoute du socket");
        exit(1);
    }

    // Mettre le socket d'écoute en mode non-bloquant
    if (fcntl(listenSocket, F_SETFL, O_NONBLOCK) < 0)
    {
        perror("Erreur lors de la configuration du socket d'écoute en mode non-bloquant");
        exit(1);
    }
    // Ajouter le socket d'écoute à la structure pollfd
    for (int j = 0; j <= MAX_CLIENTS; j++)
    {
        fds[j].fd = listenSocket;
        fds[j].events = POLLIN | POLLOUT;
        password[j] = 0;
        nickname[j] = 1;
        welcome[j] = 0;
    }
    // Boucle principale pour attendre les événements
    while (true)
    {
        int result = poll(fds.data(), fds.size(), -1); // Attendre indéfiniment
        if (result < 0)
        {
            perror("Erreur lors de l'appel à poll");
            exit(1);
        }
        // Vérifier les événements sur les descripteurs de fichiers surveillés
        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents)
            {
                if (fds[i].fd == listenSocket)
                {
                    // Événement sur le socket d'écoute (nouvelle connexion entrante)
                    clientSocket = accept(listenSocket, nullptr, nullptr);
                    if (clientSocket < 0) {
                        if (errno != EWOULDBLOCK && errno != EAGAIN) {
                            perror("Erreur lors de l'acceptation de la connexion");
                            exit(1);
                        }
                    } else {
                        // Mettre le socket client en mode non-bloquant
                        if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) < 0) {
                            perror("Erreur lors de la configuration du socket client en mode non-bloquant");
                            exit(1);
                        }

                        // Ajouter le socket client à la structure pollfd
                        fds[i].fd = clientSocket;
                        fds[i].events = POLLIN;
                        
                    }
                }
                else 
                {
                    // Événement sur un socket client existant (données reçues)
                    std::string buffer(BUFFER_SIZE, '\0');

                    int bytesRead = recv(fds[i].fd, &buffer[0], buffer.size(), 0);
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
                        close(fds[i].fd);
                        fds[i].fd = -1;
                        std::cout << user.returnNickname(i) << " déconnecté" << std::endl;
                        user.setUsername(("Client " + std::to_string(i)).c_str(), i);
                        user.setNickname(("Client " + std::to_string(i)).c_str(), i);
                    } 
                    else if (password[i] == 0)
                    {
                        if (pass == find_next_word(buffer.find("PASS") + 5, buffer))
                        {
                            password[i] = 1;
                            nickname[i] = user.setNickname(find_next_word(buffer.find("NICK") + 5, buffer), i);
                            if (nickname[i] == 1)
                                send(fds[i].fd, (":server-irc 433 * " + find_next_word(buffer.find("NICK") + 5, buffer) + " :Nickname is already in use\n").c_str(), 19 + find_next_word(buffer.find("NICK") + 5, buffer).length() + 30, 0);
                            user.setUsername(find_next_word(buffer.find("USER") + 5, buffer), i);
                            if (welcome[i] == 0)
                            {
                                send(fds[i].fd, (":server-irc 001 " + user.returnNickname(i) + " :Bienvenue sur le serveur IRC\n").c_str(), 17 + 32 + user.returnNickname(i).length(), 0);
                                welcome[i] = 1;
                            }
                        }
                        else
                        {
                            send(fds[i].fd, ":server-irc 464 client :Mot de passe requis\n", 45, 0);
                        }
                    }
                    else
                    {
                        parse_buffer(buffer, user, i);
                        std::cout << user.returnNickname(i) << " : " << buffer << std::endl;
                    }
                }
            }
        }
    }
    return 0;
}
