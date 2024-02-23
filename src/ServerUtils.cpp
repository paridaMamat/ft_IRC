/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:19:04 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/20 15:09:34 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

int	Server::createServerSocket(void)
{
	int rc, fd, on = 1;
	fd = socket(_servInfo->ai_family, _servInfo->ai_socktype, _servInfo->ai_protocol);
	if (fd == ERROR)
	{
		std::cerr << BGREEN "[Server] " << GREEN "Socket creation error" << std::endl;
		server_shutdown = TRUE;
		close(fd);
		return ERROR;
	}
	setFd(fd);
    rc = setsockopt(getFd(), SOL_SOCKET, SO_REUSEPORT, (const void*)&on, sizeof(on));
	if (rc == ERROR)
	{
		std::cerr << BGREEN "[Server] " << GREEN "Impossible to reuse the socket" << std::endl;
		close(getFd());
		server_shutdown = TRUE;
		return ERROR;
	}
	return TRUE;
}

int Server::fillServinfo(char *port)
{
	_hints.ai_family = AF_INET;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE;
	
	if (getaddrinfo(NULL, port, &_hints, &_servInfo) < 0)
	{
		std::cerr << BRED "[Server] " << RED << "Flop of addrinfo" << DEFAULT << std::endl;
		return (ERROR);
	}
	return (TRUE);
}

int	Server::bindServerSocket(void)
{
	if (bind(getFd(), _servInfo->ai_addr, _servInfo->ai_addrlen) == ERROR)
	{
		std::cerr << BRED "[Server] " << RED  "Socket impossible to bind" DEFAULT << std::endl;
		close(getFd());
		server_shutdown = TRUE;
		return ERROR;
	}
	fcntl(getFd(), F_SETFL, O_NONBLOCK);
	return (TRUE);
}

int		Server::listenForConnection(void)
{
	if (listen(getFd(), BACKLOG) ==  ERROR)
	{
		std::cerr << BRED "[Server] " << RED "Socket cannot listen" DEFAULT << std::endl;
		server_shutdown = TRUE;
		close(getFd());
		return ERROR;
	}
	std::cout << BMAGENTA "[Server] " << MAGENTA "Listening on socket fd #" << _servFd << DEFAULT << std::endl;
	return TRUE;
}

int		Server::acceptConnection(void)
{
	sockaddr_in client;
	socklen_t addr_size = sizeof(sockaddr_in);
	int cliFd = accept(getFd(), (sockaddr *)&client, &addr_size);
	if (cliFd == ERROR)
	{
		std::cerr << BRED "[Server] " << RED "Socket cannot accept connection" DEFAULT << std::endl;
		return BREAK ;
	}
	fcntl(cliFd, F_SETFL, O_NONBLOCK);
	return (cliFd);
}

int Server::initializeServer(void)
{
    if (createServerSocket() == ERROR)
		return ERROR;
    if (bindServerSocket() == ERROR)
		return ERROR;
    if (listenForConnection() == ERROR)
		return ERROR;
	std::cout << BMAGENTA "[Server] " <<  MAGENTA "Waiting for connections... " DEFAULT << std::endl;
	freeaddrinfo(_servInfo);
	return TRUE;
}

int		Server::checkRecv(std::vector<pollfd> &poll_fds, int res, std::vector<pollfd>::iterator it)
{
	if (res == ERROR)
	{
		std::cout << BRED "[Server] " << RED "recv() error\n" DEFAULT;
		return ERROR;
	}	
	if (res == 0)
	{
		std::cout << BRED "[Server] " << RED "Nothing to receive: client #" << it->fd << " is disconnected\n" DEFAULT;
		delClient(poll_fds, it, it->fd);
		return ERROR;
	}
	return TRUE;
}


void 	addToClientBufferExtended(Server *server, int cliFd, std::string reply)
{
	Client *client = server->getClient(cliFd);
	
	if (client)
		client->setRecvMsg(":" + server->getServerName() + " " + reply);
	client->sendReply(client->getFd());
}

void	addToClientBuffer(Server *server, int cliFd, std::string reply)
{
	Client *client = server->getClient(cliFd);
	
	if (!client)
		return ;	
	client->setRecvMsg(reply);
	client->sendReply(client->getFd());
}
