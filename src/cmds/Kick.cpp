/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:56:07 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/20 09:43:30 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Server.hpp"

KickCommand::KickCommand(Server *server) : Command(server) {}

KickCommand::~KickCommand() {}

void KickCommand::execute(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2)
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "KICK"));
		return;
	}

    std::string chan_name = arguments[0];
	chan_name[0] == '#' ? chan_name : chan_name.insert(0, 1, '#');
	std::string target = arguments[1];

    if (_server->isValidNickname(target))
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NOSUCHNICK(client->getPrefix(), target));
		return ;
	}
    
    if (_server->isValidChannelName(chan_name))
    {
        addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NOSUCHCHANNEL(client->getPrefix(), target));
		return ;
    }
    
    Channel* 	channel = _server->getChannel(chan_name);
	Client*		client_target = _server->getClientByNickname(target); 
	std::string reason = "";

	if (arguments.size() > 2)
	{
		reason = arguments[2];
		for (size_t i = 3; i < arguments.size(); i++)
			reason += arguments[i] + " ";
	}
    
    if (!channel->isOper(client))
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_CHANOPRIVSNEEDED(client->getPrefix(), chan_name));
		return;
	}
    if (!channel->isInChannel(client_target))
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_USERNOTINCHANNEL(client->getPrefix(), target, chan_name));
		return ;
	}	
	_server->broadcastChannel(NULL, RPL_KICK(client->getPrefix(), chan_name, target, reason), channel);
	channel->removeClient(client_target);
	channel->replyList(client);
	return ;
}
