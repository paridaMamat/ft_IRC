/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 15:19:10 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/20 17:22:13 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"
#include "../lib/Client.hpp"
#include "../lib/IrcLib.hpp"

bool server_shutdown = false;

void	Server::signal_handler(int signal)
{
	if (signal == SIGPIPE)
		return ;
	server_shutdown = true;
	std::cout << BRED "[Server] " RED << "Shutting down..." DEFAULT << std::endl;
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << GREY "Usage: " DEFAULT << av[0] << "<port> <password>\n";
		return 1;
	}
	time_t rawtime;
	struct tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	if (checkArg(av[1], av[2]) == false)
		return 2;
	Server server(av[1], av[2], timeinfo);
	if (server.fillServinfo(av[1]) == ERROR)
		return 3;
	if (server.initializeServer() == ERROR)
		return 4;
	signal(SIGINT, Server::signal_handler);
	server.manageLoop();
	delete server.getCmdHandler();
	return (0);
}
