/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skhali <skhali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 13:17:02 by valentin          #+#    #+#             */
/*   Updated: 2023/07/23 16:32:02 by skhali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"
#include "../includes/Server.hpp"
#include "../includes/User.hpp"
#include "../includes/Channel.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: ./server <port> <password>" << std::endl;
        return (0);
    }
    try {
        if (!is_port(argv[1]))
            return (1);
        Server server(std::atoi(argv[1]), argv[2]);
        server_exec(server);
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what();
    }
    return 0;
}
