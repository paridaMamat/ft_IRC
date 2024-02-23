/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:55:34 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/20 09:38:13 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Server.hpp"
#include "../../lib/Command.hpp"

InvitCommand::InvitCommand(Server *server) : Command(server) {}

InvitCommand::~InvitCommand() {}

void InvitCommand::execute(Client *client, std::vector<std::string> arguments)
{
    if (arguments.size() < 2)
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "INVITE"));
		return;
	}
	
    const std::string  target = arguments[0];
	std::string&  chan_name = arguments[1];
	chan_name[0] == '#' ? chan_name : chan_name.insert(0, 1, '#');
	
	Client*		client_target = _server->getClientByNickname(target); 
	if (!client_target)
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NOSUCHNICK(client->getNickname(), target));
		return ;
	}

	Channel* 	channel = _server->getChannel(chan_name);
    if (channel == NULL)
    {
        addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NOSUCHCHANNEL(client->getNickname(), chan_name));
		return ;
    }
    
	if (!channel->isInChannel(client))
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_NOTONCHANNEL(client->getNickname(), chan_name));
		return;
	}
	else if (!channel->isOper(client))
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_CHANOPRIVSNEEDED(client->getNickname(), chan_name));
		return ;
	}
	else if (channel->isInChannel(client_target))
	{
		addToClientBuffer(client->getServer(), client->getFd(), ERR_USERONCHANNEL(client->getNickname(), target, chan_name));
		return ;
	}
	else 
	{
		if ((channel->getL() - channel->getClients().size()) > 0)
		{
			addToClientBufferExtended(client->getServer(), client->getFd(), RPL_INVITING(client->getNickname(), chan_name, target));
			addToClientBuffer(client->getServer(), client_target->getFd(), RPL_INVITE(client->getPrefix(), target, chan_name));
			channel->addInvite(client_target);
		}
		else 
			addToClientBufferExtended(client->getServer(), client->getFd(), ERR_CHANNELISFULL(client->getPrefix(), chan_name));
	}
}
