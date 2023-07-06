/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valentin <valentin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 13:17:02 by valentin          #+#    #+#             */
/*   Updated: 2023/07/06 15:52:57 by valentin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string find_next_word(int i, std::string str)
{
    int j = i;
    while (std::isalpha(str[i]))
        i++;
    return (str.substr(j, i -j));
}

int main(int argc, char **argv)
{
    if (argc != 3)
        return (0);
    User user;
    std::string pass = argv[2];
    int port = std::atoi(argv[1]);
    int password[MAX_CLIENTS];
    int listenSocket, clientSocket;
    struct sockaddr_in serverAddress;
    std::vector<pollfd> fds(MAX_CLIENTS + 1); // +1 pour le socket d'écoute

    // Création du socket d'écoute
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0) {
        perror("Erreur lors de la création du socket d'écoute");
        exit(1);
    }

    // Configuration de l'adresse du serveur
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port); // Port d'écoute

    // Lier le socket d'écoute à l'adresse et au port spécifiés
    if (bind(listenSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0) {
        perror("Erreur lors de la liaison du socket d'écoute");
        exit(1);
    }

    // Mettre le socket d'écoute en mode écoute
    if (listen(listenSocket, MAX_CLIENTS) < 0) {
        perror("Erreur lors de la mise en écoute du socket");
        exit(1);
    }

    // Mettre le socket d'écoute en mode non-bloquant
    if (fcntl(listenSocket, F_SETFL, O_NONBLOCK) < 0) {
        perror("Erreur lors de la configuration du socket d'écoute en mode non-bloquant");
        exit(1);
    }
    // Ajouter le socket d'écoute à la structure pollfd
    for (int j = 0; j <= MAX_CLIENTS; j++)
    {
        fds[j].fd = listenSocket;
        fds[j].events = POLLIN | POLLOUT;
        password[j] = 0;
    }
    // Boucle principale pour attendre les événements
    while (true) {
        int result = poll(fds.data(), fds.size(), -1); // Attendre indéfiniment
        if (result < 0) {
            perror("Erreur lors de l'appel à poll");
            exit(1);
        }

        // Vérifier les événements sur les descripteurs de fichiers surveillés
        for (size_t i = 0; i < fds.size(); i++) {
            if (fds[i].revents) {
                if (fds[i].fd == listenSocket) {
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
                        //send(fds[i].fd, ":serveur-irc 464 client :Mot de passe requis", 45, 0);
                        
                    }
                } else {
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
                        user.printUsername(i); std::cout << " déconnecté" << std::endl;
                    } 
                    else if (password[i] == 0)
                    {
                        if (pass == find_next_word(buffer.find("PASS") + 5, buffer))
                        {
                            
                            //send(fds[i].fd, "Bienvenu sur le serveur\n", 25, 0);
                            send(fds[i].fd, ":serveur-irc 001 valentin :Bienvenue sur le serveur IRC\n", 57, 0);
                            user.setNickname(find_next_word(buffer.find("NICK") + 5, buffer), i);
                            user.setUsername(find_next_word(buffer.find("USER") + 5, buffer), i);
                            user.printUsername(i); std::cout << " connecté" << std::endl;
                            user.printUsername(i); std::cout << " : " << buffer << std::endl;
                           
                            password[i] = 1;
                        }
                        else
                        {
                            //fds[i].events = POLLOUT;
                            send(fds[i].fd, ":serveur-irc 464 client :Mot de passe requis\n", 46, 0);
                        }
                    }
                    else
                    {
                        
                        user.printUsername(i); std::cout << " : " << buffer << std::endl;
                    }
            }
            }
        }
    }

    return 0;
}
