/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 18:11:32 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/15 18:34:07 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Client.hpp"

NoticeCommand::NoticeCommand(Server *server) : Command(server) {}

NoticeCommand::~NoticeCommand() {}

void NoticeCommand::execute(Client *client, std::vector<std::string> arguments)
{
		if (arguments.size() < 2 || arguments.size() == 0)
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "PRIVMSG"));
		return;
	}
	
	const std::string  target = arguments[0];
	
	std::string message = arguments[1];
	for (size_t i = 2; i < arguments.size(); i++)
		message += " " + arguments[i];
	if (target[0] == '#')
	{
		if (_server->isValidChannelName(target))
		{
			addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NOSUCHCHANNEL(client->getNickname(), target));
			return ;
		}
		Channel* channel = _server->getChannel(target);
		channel->broadcastChannelmessage(NULL, RPL_NOTICE(client->getPrefix(), channel->getName(), message));
		return ;
	}
	if (_server->isValidNickname(target))
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NOSUCHNICK(client->getNickname(), target));
		return ;
	}
	
	Client*		client_target = _server->getClientByNickname(target);
	addToClientBuffer(client->getServer(), client_target->getFd(), RPL_NOTICE(client->getPrefix(), client_target->getNickname(), message));	
}
