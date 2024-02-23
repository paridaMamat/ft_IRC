/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:45:16 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/20 15:10:31 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

Server::Server(std::string port, std::string pwd, struct tm * time) : _servInfo(NULL)
{
 	_servFd = -1;
	_servPort = atoi(port.c_str());
	_cliNb = 0;
	_servPwd = pwd;
	_servName = "IrcServer";
	this->setDatetime(time);
	_result = 0;
	memset(&_hints, 0, sizeof(_hints));
	_handler = new CmdHandler(this);
}

Server::~Server()
{
	close(_servFd);
	if (_clients.empty())
	{
        _clients.clear();
        return;
    }
	std::map<int, Client *>::iterator it = _clients.begin();
	for(; it != _clients.end(); ++it)
	{
		delete it->second;
		close(it->first);
	}
	_clients.clear();
	if (_channels.empty())
    {
        _channels.clear();
        return;
    }
    for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
       delete *it;
    _channels.clear();
}

/*********************  Assessors !!  ************************/

Channel* Server::getChannel(const std::string& cName)
{
    for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if ((*it)->getName() == cName)
            return *it; // Channel found, return the pointer
    }
    return NULL; // Channel not found, return NULL
}

Client*			Server::getClient(int fd){return _clients[fd];}

std::string		&Server::getPwd(void){ return _servPwd;}

int				&Server::getPort(void){ return _servPort;}

int				&Server::getFd(void){ return _servFd;}

std::string		&Server::getServerName(void){return _servName;}

std::string		&Server::getStartTime(void){ return _time;}

Client 			*Server::getClientByNickname(const std::string &nickname)
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) 
	{
		if (it->second->getNickname() == nickname) 
			return it->second;
	}
	return NULL; 
}

void		Server::setFd(int fd){_servFd = fd;}

void		Server::setDatetime(struct tm *timeinfo)
{
	char buffer[80];

	strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
  	std::string str(buffer);
	_time = str;
}

void 		Server::broadcastChannel(Client* client, std::string message, Channel* channel)
{
	if (channel)
		channel->broadcastChannelmessage(client, message);
}

void 		Server::addChannel(std::string chan_name, Channel* channel)
{
    if (channel)
    {
        // Check if the channel name already exists
        std::vector<Channel*>::iterator it;
        for (it = _channels.begin(); it != _channels.end(); ++it)
        {
            if ((*it)->getName() == chan_name)
            {
                // If the channel name already exists, print an error message
                std::cout << "Channel name '" << chan_name << "' is already in use." << std::endl;
                return;
            }
        }
        // If the channel name doesn't already exist, add the channel to the list
        _channels.push_back(channel);
    }
}
