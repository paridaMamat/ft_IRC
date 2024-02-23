/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmaimait <pmaimait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 12:03:37 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/20 16:30:47 by pmaimait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include "IrcLib.hpp"
#include "Server.hpp"
#include "Client.hpp"

class Server;
class Client;

class Channel
{
	public:
		Channel(std::string const &name, std::string const &password, Server *server);
		~Channel();

		/***********   Assessors   ***********/
		const std::string 			getName(void) const {return _name;};
		const std::string 			&getTopic(void) const {return _topic;};
		std::string					getPassword(){return _password;};
		std::vector<Client*>        getOperator(void){return _ops;};
		std::vector<Client*>  		getClients(){return _clients;};
		std::vector<std::string>	getNicknames();
		Server*						getServer(){return _server;};
		Client*						getClient(const std::string &nickname);
		Client*						getAdmin(){return _admin;};
		size_t						getL(){return _l;};
		bool						getI(){return _i;};
		bool						getT(){return _t;};
				
		void 						setTopic(const std::string &topic);
		void						setPassword(std::string password){_password = password;};
		void						setL(size_t N){_l = (N != 0) ? N : std::numeric_limits<int>::max(); };
		void						setI(int b){_i = b;};
		void						setT(int b){_t = b;};
		
		/***********   Functions   ***********/
		bool    					isOper(Client *client);
		bool						isInChannel(Client *client);
		bool						isInvited(Client *client);
		void 						addClient(Client *cli);
		void 						addAdmin(Client *cli){_admin = cli;};
		void						addOperator(Client *client){_ops.push_back(client);};
		void						addInvite(Client *client){_invited.push_back(client);};
		void 						removeOpe(Client *client);
   		void 						partChannel(Client* cli, std::string reason);
		void						removeClient(Client* client);
		void 	  					broadcastChannelmessage(Client* client, std::string message);
		void 						broadcastChannelmessageExt(Client* client, std::string message);
		void 	  					broadcastChannelPrimsg(Client* client, std::string message);
		void						replyList(Client* client);	
		
	private:
		Channel(Channel const &name);
		Channel &operator=(Channel const &c);
		
		const std::string	_name;
		std::string			_topic;
		std::string			_password;
		Server*				_server;
		std::vector<Client *>_clients;
		std::vector<Client *>_ops;
		std::vector<Client *>_invited;
		Client* 			_admin;
		size_t				_l ;		//Set/remove the user limit to channel
		bool				_i ;     	//Set/remove Invite-only channel, true is invite only
		bool				_t ;		//Set/remove the restrictions of the TOPIC command to channel operator    //true is only operator can set topic
		                        											
};

#endif
