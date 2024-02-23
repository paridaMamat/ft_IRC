/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:55:54 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/20 15:14:30 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Client.hpp"

JoinCommand::JoinCommand(Server *server) : Command(server) {}

JoinCommand::~JoinCommand() {}

void JoinCommand::execute(Client *client, std::vector<std::string> arguments)
{
    if (arguments.empty())
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "JOIN"));
		return;
	}
    
	std::string name = arguments[0];

	if (name == "0")
	{
		client->partAllChannel();
		return;
	}
		
	name = parseChannelName(arguments[0]);
	std::string password = arguments.size() > 1 ? arguments[1] : "";

	Channel* channel = _server->getChannel(name);
	if (channel == NULL)
	{
        Channel* channel = new Channel(name, password, _server);
		addToClientBufferExtended(client->getServer(), client->getFd(), RPL_JOIN(client->getPrefix(), name));
		_server->addChannel(name, channel);
		channel->addClient(client);
		channel->addOperator(client);
		client->addChannel(channel);
		channel->replyList(client);
		channel->addAdmin(client);
	}
    else 
	{
		if(channel->getI() == true && channel->isInvited(client) == false)
		{
			addToClientBufferExtended(client->getServer(), client->getFd(), ERR_INVITONLYCHAN(client->getPrefix(), name));
			return ;
		}
		else if (((channel->getL() - channel->getClients().size()) > 0))
		{
			if (password == channel->getPassword())
			{
				channel->addClient(client);
				client->addChannel(channel);
				addToClientBuffer(client->getServer(), client->getFd(), RPL_JOIN(client->getPrefix(), name));
				channel->replyList(client);
				_server->broadcastChannel(client, RPL_JOIN(client->getPrefix(), name), channel);
				if (!channel->getTopic().empty())
					addToClientBufferExtended(client->getServer(), client->getFd(), RPL_TOPIC(client->getNickname(), channel->getName(), channel->getTopic()));
				return;
			}
			else
				addToClientBufferExtended(client->getServer(), client->getFd(), ERR_BADCHANNELKEY(client->getNickname(), name));
			
		}
		else
			addToClientBufferExtended(client->getServer(), client->getFd(), ERR_CHANNELISFULL(client->getNickname(), name));
	}
}
