/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 14:30:29 by pmaimait          #+#    #+#             */
/*   Updated: 2024/02/20 15:16:18 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "IrcLib.hpp"
# include <string>
# include <numeric>

class Server;
class Client;
class Channel;

class Command
{
    protected :
        Server *_server;
		bool 	_authRequired;
    public :
        explicit Command(Server *server, bool authRequired = true) : _server(server), _authRequired(authRequired) {};
        virtual ~Command(){ _server = NULL;};
		
		bool getAuthRequired() const { return _authRequired; };
        virtual void execute(Client *client, std::vector<std::string> arguments) = 0;  
};

class NoticeCommand : public Command
{
public:
	NoticeCommand(Server *server);
	~NoticeCommand();

	void execute(Client *client, std::vector<std::string> arguments);
};

class PrivMsgCommand : public Command
{
public:
	PrivMsgCommand(Server *server);
	~PrivMsgCommand();

	void execute(Client *client, std::vector<std::string> arguments);
	void executeBot(Client *client, std::vector<std::string> arguments);
};

class PartCommand : public Command
{
public:
	PartCommand(Server *server);
	~PartCommand();

	void execute(Client *client, std::vector<std::string> arguments);
	void	broadcastToAllChannelMembers(Server *server, Channel &channel);
};

class QuitCommand : public Command
{
public:
	QuitCommand(Server *server, bool auth);
	~QuitCommand();

	void execute(Client *client, std::vector<std::string> arguments);
};

class JoinCommand : public Command
{
public:
	JoinCommand(Server *server);
	~JoinCommand();

	void execute(Client *client, std::vector<std::string> arguments);
};

class UserCommand : public Command
{
public:
	UserCommand(Server *server, bool auth);
	~UserCommand();

	void execute(Client *client, std::vector<std::string> arguments);
};

class NickCommand : public Command
{
public:
	NickCommand(Server *server, bool auth);
	~NickCommand();
	char _i;
	char _j;	
	void execute(Client *client, std::vector<std::string> arguments);
};

class PassCommand : public Command
{
public:
	PassCommand(Server *server, bool auth);
	~PassCommand();

	void execute(Client *client, std::vector<std::string> arguments);
};

class KickCommand : public Command
{
public:
	KickCommand(Server *server);
	~KickCommand();

	void execute(Client *client, std::vector<std::string> arguments);
};

class PingCommand : public Command
{
public:
	PingCommand(Server *server);
	~PingCommand();

	void execute(Client *client, std::vector<std::string> arguments);
};

class ModeCommand : public Command
{
public:
	ModeCommand(Server *server);
	~ModeCommand();

	void execute(Client *client, std::vector<std::string> arguments);
};

class InvitCommand : public Command
{
public:
	InvitCommand(Server *server);
	~InvitCommand();

	void execute(Client *client, std::vector<std::string> arguments);
};

class TopicCommand : public Command
{
public:
	TopicCommand(Server *server);
	~TopicCommand();

	void execute(Client *client, std::vector<std::string> arguments);
};

class WhoIsCommand : public Command
{
public:
	WhoIsCommand(Server *server);
	~WhoIsCommand();

	void execute(Client *client, std::vector<std::string> arguments);
};

bool			isAllDigits(const std::string& str);
std::string		retrieveChannelOp(Client *client);
std::string		retrieveChannelNames(Client *client);
std::string 	parseChannelName(std::string chan);
std::string 	parseNickname(std::string name);
void 			quitAllChannels(Client *cli, std::string reason);

#endif