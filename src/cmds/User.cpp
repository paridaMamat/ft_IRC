/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:05:48 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/19 14:45:07 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Client.hpp"

UserCommand::UserCommand(Server *server, bool auth) : Command(server, auth) {}

UserCommand::~UserCommand() {}

void UserCommand::execute(Client *client, std::vector<std::string> arguments) 
{

	if (!client->getUsername().empty())
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_ALREADYREGISTERED(client->getPrefix()));
		return;
	}
	if (arguments.size() < 4) 
	{
		addToClientBufferExtended(client->getServer(), client->getFd(), ERR_NEEDMOREPARAMS(client->getNickname(), "USER"));
		return;
	}
	std::string user = arguments[0];
	if (user.size() > 9)
		user.erase(9, user.size() - 9);
	client->setUsername(user);
	
	std::string name, lastname;
	name = arguments[3];
	name[0] == ':' ? name.erase(0, 1) : name;
	if (arguments.size() == 5)
	{
		lastname = name + " " + arguments[4];
		if (lastname.size() > 20)
			lastname.erase(20, lastname.size() - 20);
		client->setRealName(lastname);
	}
	else if (arguments.size() == 4)
	{
		if (name.size() > 9)
			name.erase(9, name.size() - 9);
		client->setRealName(name);
	}
	if (!client->getNickname().empty())
		addToClientBuffer(client->getServer(), client->getFd(), RPL_NICK(client->getOldNick(), client->getUsername(), client->getNickname()));
	client->welcomeClient(client->getServer());
}
