/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:43:39 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/20 13:31:52 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include "Server.hpp"
#include "IrcLib.hpp"
#include "Channel.hpp"

class Server;
class Channel;
 
class Client
{
	public:
		Client(int fd);
		~Client();
		
		/**********    Structures    *********/
		typedef struct s_names // all the infos allowing to identify a client
		{
			std::string		_nickname; // pseudo: usefull for channel operator
			std::string		_oldNick; //if it's not the 1st nickname
			std::string		_username; // user = personn who is using IRC Client software
			std::string 	_realname;
			std::string		_pwd;
			std::string		_host; // IP adress: usefull for channel operator
		} t_names;

		typedef struct s_status // the client status to check what to do
		{
			bool			_connectionPwd;
			bool			_registred;
			bool			_welcomed;
			bool			_toDisconnect;
		} t_status;
		
		/**********    Assessors     *********/
		std::string	const 	&getNickname(void)const;
		std::string	const 	&getOldNick(void)const { return _infos._oldNick;};
		std::string	const 	&getUsername(void)const ;
		std::string const 	&getRealName()const ;
		std::string	const 	&getHost(void)const ;
		std::string		 	getMsgRecvd(void)const ;
		std::string		 	getPartialMsg(void)const ;
		std::string		 	getFullMsg(void)const ;
		std::string		 	getMsgSent(void)const ;
		std::string			getPrefix(void)const ;
		bool const			&getConnPwd(void) const ;
		bool const			&getWelcomeStatus(void) const ;
		bool const			&getRegistrationStatus(void)const ;
		bool const			&getDeconnStatus(void)const ;
		int	const			&getFd(void)const ;
		Server				*getServer(void);
		std::deque<Channel*> &getChannels(void){return _channel;};
		void				setServer(Server *serv){_server = serv;};
		void				setNickname(std::string);
		void			 	setOldNick(std::string nick){_infos._oldNick = nick;};
		void				setUsername(std::string);
		void				setRealName(std::string);
		void				setPwd(std::string pwd);
		void				setDeconnStatus(bool);
		void				setWelcomeStatus(bool);
		void 				setRecvMsg(std::string msg);
		void				setPartialMsg(std::string partialMsg);
		void				setFullMsg(std::string partialMsg);
		void				resetPartialMsg(void);
		void				resetFullMsg(void);
		void				resetRecvMsg(void);
;		
		/**********    Messages Management     *********/
		bool				isRegistred(void);
		void				sendMsgtoServer(std::string msg);
		void				recvMsgfromServer(void);
		void				welcomeClient(Server *serv);			
		bool				sendReply(int fd);
		
		/**********    Channel Management    *********/
		Channel*			getActiveChannel(void) const;
		void				addChannel(Channel* channel);
		void				partAllChannel(void);
		void				delChannel(Channel *channel);
		
	private:
		Client(Client const &s);
		Client &operator=(Client const &s);
		
		int						_cliFd;
		std::string				_partialMsg;
		std::string				_fullMsg;
		std::string				_recvdFromServ;
		t_names					_infos; 
		t_status				_state;
		Server*					_server;
		std::deque<Channel*> 	_channel;
};

void	addToClientBufferExtended(Server *server, int cliFd, std::string reply);
void	addToClientBuffer(Server *server, int cliFd, std::string reply);

#endif


