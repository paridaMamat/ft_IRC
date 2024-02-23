/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:52:00 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/20 15:16:05 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/Command.hpp"
#include "../../lib/IrcLib.hpp"
#include "../../lib/Server.hpp"

void PrivMsgCommand::executeBot(Client *client, std::vector<std::string> arguments)
{
    
    if (arguments.size() < 2)
    {
        addToClientBuffer(client->getServer(), client->getFd(), "Bot response: Please provide a command for the bot.");
        return;
    }

    std::string botCommand = arguments[1];
    std::string prefix = "Bot!botuser@IrcServer";

    if (botCommand == ":info")
        addToClientBuffer(client->getServer(), client->getFd(), RPL_PRIVMSG(prefix, client->getNickname(), "I am a simple IRC bot."));
    else if (botCommand == ":joke")
        addToClientBuffer(client->getServer(), client->getFd(), RPL_PRIVMSG(prefix, client->getNickname(), "Why don't scientists trust atoms? Because they make up everything!"));
    else if (botCommand == ":time")
    {
        // get the actual time
        std::time_t timeNow = std::time(NULL);
        std::string timeStr = std::ctime(&timeNow);
        timeStr.erase(timeStr.length() - 1);

        addToClientBuffer(client->getServer(), client->getFd(), RPL_PRIVMSG(prefix, client->getNickname(), "Current time is " + timeStr));
    }
    else if (botCommand == ":quote")
    {
        // add a random quote
        std::vector<std::string> quotes;
        quotes.push_back("To be or not to be, that is the question.");
        quotes.push_back("The only limit is your imagination.");
        quotes.push_back("Stay hungry, stay foolish.");

        srand(static_cast<unsigned int>(time(NULL)));
        int randomIndex = rand() % quotes.size();
        addToClientBuffer(client->getServer(), client->getFd(), RPL_PRIVMSG(prefix, client->getNickname(), "Quote of the day - " + quotes[randomIndex]));
    }
    else
        addToClientBuffer(client->getServer(), client->getFd(), RPL_PRIVMSG(prefix, client->getNickname(), "Unknown command. Supported commands: info, joke, time, quote"));
}
