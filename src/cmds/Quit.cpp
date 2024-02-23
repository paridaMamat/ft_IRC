/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 17:32:59 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/20 15:12:07 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Server.hpp"

QuitCommand::QuitCommand(Server *server, bool auth) : Command(server, auth){}

QuitCommand::~QuitCommand() {}

void QuitCommand::execute(Client *client, std::vector<std::string> arguments)
{
	std::string  reason = "";
	
	
	if (!arguments.empty() || arguments.size() > 0)
	{
		reason = arguments[0];
		for (size_t i = 1; i < arguments.size(); i++)
			reason += " " + arguments[i];
		reason[0] == ':' ? reason : reason.insert(0, 1, ':');
	}
	addToClientBuffer(client->getServer(), client->getFd(), RPL_QUIT(client->getPrefix(), reason));
	client->setDeconnStatus(true);
}
