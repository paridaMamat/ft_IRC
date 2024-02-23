/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 12:06:20 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/20 16:56:02 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Channel.hpp"

Channel::Channel(std::string const &name, std::string const &password, Server *server)
					: _name(name),_password(password), _server(server)
{
    _clients.clear();
    _ops.clear();
    _invited.clear();
    _topic = "";
    _l = MAXCONN;
    _i = FALSE;
    _t = TRUE;
}
                    
Channel::~Channel() {}

std::vector<std::string> Channel::getNicknames()
{
    std::vector<std::string> nicknames;
    std::vector<Client*>::iterator it = _clients.begin();

    while (it != _clients.end())
    {
        Client* client = *it;

        // Check if client is in the _ops vector
        if (std::find(_ops.begin(), _ops.end(), client) != _ops.end()) {
            nicknames.push_back("@");  // Client is in _ops
        } else {
            nicknames.push_back("");   // Client is not in _ops
        }

        nicknames.back() += client->getNickname();
        it++;
    }
    return nicknames;
}

Client  *Channel::getClient(const std::string &nickname)
{
	std::vector<Client *>::iterator it = _clients.begin();

	while (it != _clients.end())
	{
		if ((*it)->getNickname() == nickname)
			return *it;
		it++;
	}
	return NULL;
}

void 	 Channel::setTopic(const std::string &topic)
{
    _topic = topic;
}

void Channel::addClient(Client *cli)
{
    // Check if the client is already in the channel
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it == cli)
        {
            // If the client is already in the channel, print an error message and return
            addToClientBuffer(cli->getServer(), cli->getFd(), ERR_USERONCHANNEL(cli->getPrefix(), cli->getNickname(), getName()));	
            return;
        }
    }
    // If the client is not already in the channel, add it to the list
    _clients.push_back(cli);
}


bool    Channel::isOper(Client *client)
{
	std::vector<Client *> opers_chan = this->getOperator();
	std::vector<Client *>:: iterator it_oper = opers_chan.begin();

	while (it_oper != opers_chan.end())
	{
		Client *oper = *it_oper;
		if (oper == client)
			return TRUE;
		++it_oper;
	}
	return FALSE;
}


void Channel::partChannel(Client* cli, std::string reason)
{
    
    addToClientBuffer(cli->getServer(), cli->getFd(), RPL_PART(cli->getPrefix(), getName(), reason));
    _server->broadcastChannel(cli, RPL_PART(cli->getPrefix(), getName(), reason), this);
    // if (cli->getNickname() == _admin->getNickname())
    //     addToClientBuffer(cli->getServer(), cli->getFd(), RPL_KICK(cli->getPrefix(), getName(), cli->getNickname(), reason));
    removeClient(cli);
    replyList(cli);
    
    if (_clients.size() == 0)
    {
        //addToClientBuffer(_server, cli->getFd(), RPL_ENDOFNAMES(cli->getNickname(), this->getName()));
       _server->delChannel(this);
       return;
    }  
 
    if (_ops.size() == 0 && _clients.size() > 0)
    {
        Client* client = _clients.front();

        addToClientBufferExtended(client->getServer(), client->getFd(), MODE_USERMSG(client->getNickname(), "+o"));
		_server->broadcastChannel(NULL, RPL_MODE(client->getPrefix(), getName(), "+o", client->getNickname() + " got operator privilege now"), this);
        addOperator(client);
        return;
    }
}

void    Channel::removeClient(Client* cli)
{
    bool clientFound = false;

    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if ((*it)->getNickname() == cli->getNickname())
        {
            _clients.erase(it); // the channel remove cli from its clients list
            if (isOper(cli)) 
                removeOpe(cli); // the channel remove cli from its op list
            cli->delChannel(this); // client remove channel from it channel list
            clientFound = true;
            break;
        }
    }
    if (clientFound == false)
        addToClientBufferExtended(cli->getServer(), cli->getFd(), ERR_USERNOTINCHANNEL(cli->getNickname(), "", this->getName()));
}

void Channel::removeOpe(Client *client)
{
    std::vector<Client*>::iterator it;
    for (it = _ops.begin(); it != _ops.end(); ++it)
    {
        if ((*it)->getNickname() == client->getNickname())
        {
            _ops.erase(it); // delete from list of operators in this channel
            return ;
        }
    }
    if (it == _ops.end())
        addToClientBufferExtended(client->getServer(), client->getFd(), ERR_CHANOPRIVSNEEDED(client->getNickname(), getName()));
}

bool	Channel::isInChannel(Client *client)
{
	for  (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it == client)
			return true;
	}
	return false;
}

bool        Channel::isInvited(Client *client)
{
    for  (std::vector<Client *>::iterator it = _invited.begin(); it != _invited.end(); ++it)
	{
		if (*it == client)
			return true;
	}
	return false;
}

void 	Channel::broadcastChannelPrimsg(Client* client, std::string message)
{
    std::vector<Client*> cli_target = getClients();
	std::vector<Client*>::iterator it = cli_target.begin();

    for (; it != cli_target.end(); ++it)
    {
		if (client != *it)
			addToClientBuffer(getServer(), (*it)->getFd(), RPL_PRIVMSG(client->getPrefix(), getName(), message));
	}
}

void 	Channel::broadcastChannelmessage(Client* client, std::string message)
{
    std::vector<Client*> cli_target = getClients();
	std::vector<Client*>::iterator it = cli_target.begin();
	
   for (; it != cli_target.end(); ++it)
   {
		if (client != *it)
			addToClientBuffer(getServer(), (*it)->getFd(), message);
	}
}

void 	Channel::broadcastChannelmessageExt(Client* client, std::string message)
{
    std::vector<Client*> cli_target = getClients();
	std::vector<Client*>::iterator it = cli_target.begin();
	
   for (; it != cli_target.end(); ++it)
   {
		if (client != *it)
			addToClientBufferExtended(getServer(), (*it)->getFd(), message);
	}
}

void        Channel::replyList(Client* client)
{
    std::vector<std::string> nickname = getNicknames();
    std::string list = "";
    for (std::vector<std::string>::iterator it = nickname.begin(); it != nickname.end(); ++it)
    {
        if (!it->empty()) 
        list += *it + " ";
    } 
    list += "\n";
    broadcastChannelmessage(NULL, RPL_NAMREPLY(client->getNickname(), this->getName(), list));
    broadcastChannelmessage(NULL, RPL_ENDOFNAMES(client->getNickname(), this->getName()));
}
