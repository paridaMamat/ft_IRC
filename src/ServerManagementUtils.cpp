/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagementUtils.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:18:36 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/20 13:38:59 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

int 	Server::addConnections(std::vector<pollfd>&poll_fds, std::vector<pollfd> &new_poll)
{
	(void)poll_fds;
	int 	cliFd;
	Client *cli;
	
	cliFd = acceptConnection();
	if (cliFd == ERROR)
	{
		std::cout << BGREEN "[Server] " <<  GREEN "Coudn't accept incoming connection.\n" DEFAULT;
		return BREAK ;
	}
	if (cliFd > MAXCONN + 3)
	{
		cantAddClient(cliFd);
		return ERROR ;
	}
	cli = new Client(cliFd);
	cli->setServer(this);
	cli->setPwd(getPwd());
	if (cliFd <= MAXCONN + 3)
		addClient(new_poll, cli);
	return cliFd;
}

int			Server::receiveMsg(std::vector<pollfd> &poll_fds, std::vector<pollfd>::iterator it)
{
	(void)poll_fds;
	char	buf[MAXBUF];
	Client *cli = _clients[it->fd];
	
	memset(buf, 0, MAXBUF);
	_result = recv(it->fd, buf, MAXBUF, 0);
	if (checkRecv(poll_fds, _result, it) == ERROR)
		return BREAK;

	if (_result <= MAXBUF && buf[_result - 1] != '\n')
	{
		cli->setPartialMsg(buf);
		std::cout << BBLUE "[Client] " << BLUE "Partial message from Client #" \
			<< it->fd << DEFAULT " << " << cli->getPartialMsg() << std::endl;
	}
	else if (_result <= MAXBUF)
	{
		buf[_result] = '\0';
		std::string msg = cli->getPartialMsg();
		cli->setFullMsg(msg + buf);
		cli->resetPartialMsg();
		std::cout << BBLUE "[Client] " << BLUE "Message from Client #" << it->fd 
			<< DEFAULT " << " << cli->getFullMsg();
	}
	memset(buf, 0, sizeof(MAXBUF));
	return TRUE;
}

void		Server::delClient(std::vector<pollfd> &poll_fds, std::vector<pollfd>::iterator it, int fd)
{
	Client *tmp = this->_clients[fd];
	
	quitAllChannels(tmp, " ");
	this->_clients.erase(fd);
	delete tmp;
	poll_fds.erase(it);
	_cliNb--;
	if (_cliNb <= 0)
		_cliNb = 0;
	close(fd);
	std::cout << BGREEN "[Server] " <<  GREEN "Client #" << fd
	<< " successfully disconnected. There is now " << _cliNb << " active connections." DEFAULT << std::endl;
}

void 	quitAllChannels(Client *cli, std::string reason)
{
	std::deque<Channel *> chan = cli->getChannels();
	if (chan.empty())
    {
        chan.clear();
        return;
    }
	else
	{
		std::deque<Channel *>::iterator it = chan.begin();
		for (;it != chan.end() ; it++)
		{
			(*it)->removeClient(cli);
			std::cout << "size of clients in the channel is " << (*it)->getClients().size() << std::endl;
			if ((*it)->getOperator().size() == 0 && (*it)->getClients().size() > 0)
			{
				Client* client = (*it)->getClients().front();
				addToClientBufferExtended(client->getServer(), client->getFd(), MODE_USERMSG(client->getNickname(), "+o"));
				(*it)->broadcastChannelmessage(NULL, RPL_MODE(cli->getPrefix(),(*it)->getName(), "+o", client->getNickname() + " got operator privilege now"));
				(*it)->addOperator(client);
			}
			else if ((*it)->getClients().size() == 0)
				(*it)->getServer()->delChannel(*it);
		}
		addToClientBuffer(cli->getServer(), cli->getFd(), RPL_QUIT(cli->getPrefix(), reason));	
	}	
}

void		Server::delChannel(Channel *chan)
{
	std::vector< Channel *>::iterator it = _channels.begin();
	Channel * tmp = chan;
    while(it != _channels.end())
    {
		if (*it == chan)
		{
			_channels.erase(it);
			delete tmp;
			break ;
		}
		++it;
	}
}

void		Server::addClient(std::vector<pollfd> &poll_fds, Client *cli)
{
	pollfd	cliPoll;
	
	cliPoll.fd = cli->getFd();
	cliPoll.events = POLLIN | POLLOUT;
	poll_fds.push_back(cliPoll);
	_clients.insert(std::pair<int, Client*>(cli->getFd(), cli));
	_cliNb++;
}

void		Server::cantAddClient(int cliSocket)
{
	(void) cliSocket;
	std::cout << BRED "[Server] " <<  RED "You cannot join, the server is already full." << DEFAULT << std::endl;
	send(cliSocket, "[Server] You cannot join, the server is already full", 53, 0);
	close (cliSocket);
	//close(_servFd); // we close the listening socket as we cannot add more clients
}
