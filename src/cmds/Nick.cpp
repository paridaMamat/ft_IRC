/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:02:24 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/20 15:13:18 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/IrcLib.hpp"
#include "../../lib/Client.hpp"

NickCommand::NickCommand(Server *server, bool auth) : Command(server, auth) { _i = '0'; _j = '0';}

NickCommand::~NickCommand() {}

void NickCommand::execute(Client *client, std::vector<std::string> arguments)
{
	_server = client->getServer();
	if (arguments.empty() || arguments[0].empty())
	{
		addToClientBufferExtended(_server, client->getFd(), ERR_NONICKNAMEGIVEN(client->getPrefix()));
		return;
	}

	std::string nickname = arguments[0];
	nickname = parseNickname(arguments[0]);
	
	if (nickname == "bot")
	{
		while (_server->getClientByNickname(nickname))
		{
			nickname.insert(nickname.size(), 1, _j);
			_j++;
		}
	}

	if (_server->getClientByNickname(nickname) && client->getNickname().empty())
	{
		while (_server->getClientByNickname(nickname))
		{
			nickname.insert(nickname.size(), 1, _i);
			_i++; 
		}
	}

	else if (_server->getClientByNickname(nickname))
	{
		addToClientBufferExtended(_server, client->getFd(), ERR_NICKNAMEINUSE(client->getPrefix(), nickname));
		return;
	}
	
	if (client->isRegistred() == FALSE)
    {
        client->setNickname(nickname);
        client->setOldNick(nickname);
    }
    else
    {
        client->setOldNick(client->getNickname());
        std::cout << BGREEN "[Server]" DEFAULT << GREEN "Nickname change registered. Old nickname is now : " << client->getOldNick() << DEFAULT << std::endl;
        
        client->setNickname(nickname);
    }
    
    if (!client->getUsername().empty())
        addToClientBuffer(_server, client->getFd(), RPL_NICK(client->getOldNick(), client->getUsername(), nickname));
    client->welcomeClient(_server);
}
