/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:43:36 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/20 15:11:20 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Client.hpp"

Client::Client(int fd)
{
	_partialMsg = "";
	_recvdFromServ = "";
   	_cliFd = fd;
	_infos._host = "localhost";
	_state._connectionPwd = FALSE;
	_state._registred = FALSE;
	_state._welcomed = FALSE;
	_state._toDisconnect = FALSE;
}

Client::~Client(){}

std::string	const 	&Client::getNickname(void) const {return (_infos._nickname);}

std::string	const 	&Client::getUsername(void) const {return (_infos._username);}

std::string const 	&Client::getRealName()const {return (_infos._realname);}

std::string	const 	&Client::getHost(void)const  {return (_infos._host);}

std::string		 	Client::getMsgRecvd(void)const  {return (_recvdFromServ);}

std::string		 	Client::getFullMsg(void)const {return (_fullMsg);}

std::string			Client::getPartialMsg(void)const  {return (_partialMsg);}

bool const			&Client::getRegistrationStatus(void)const {return _state._registred;}

bool	const		&Client::getConnPwd(void)const {return _state._connectionPwd;}

bool	const		&Client::getDeconnStatus(void)const{return _state._toDisconnect;}

bool const			&Client::getWelcomeStatus(void) const {return _state._welcomed;}

int	const			&Client::getFd(void)const{return _cliFd;}

Server				*Client::getServer(void){return _server;}

void				Client::setPartialMsg(std::string partialMsg){ _partialMsg += partialMsg;}

void				Client::setFullMsg(std::string fullMsg){ _fullMsg = fullMsg;}

void				Client::setNickname(std::string nick){_infos._nickname = nick;}

void				Client::setUsername(std::string user){_infos._username = user;}

void				Client::setRealName(std::string realUser){_infos._realname = realUser;}

//void				Client::setHost(std::string hostname){_infos._host = hostname;}

void				Client::setRecvMsg(std::string msg){_recvdFromServ += msg;}

void				Client::setPwd(std::string pwd){_infos._pwd = pwd;}

void				Client::setDeconnStatus(bool b){_state._toDisconnect = b;}

void				Client::setWelcomeStatus(bool b){_state._welcomed = b;}

void				Client::addChannel(Channel* channel){_channel.push_back(channel);}

void				Client::resetPartialMsg(void) { _partialMsg = "";}

void				Client::resetFullMsg(void) { _fullMsg = "";}

void				Client::resetRecvMsg(void) { _recvdFromServ = "";}

std::string  		Client::getPrefix(void)const
{
	if (this->getNickname().empty())
		return ("*");
	return _infos._nickname + "!" + _infos._username + "@" + _infos._host;
}

Channel*		Client::getActiveChannel(void) const
{
    if (!_channel.empty())
        return (_channel.back()); 
    return NULL;
}

void			Client::delChannel(Channel* channel)
{
	std::deque<Channel *>::iterator it = _channel.begin();
	for(; it != _channel.end(); it++)
	{
		if (*it == channel)
		{
    		_channel.erase(it);
			break ;
		}
	}
}

bool				Client::sendReply(int fd)
{
	int res(0);
	std::string buff = getMsgRecvd();
	if (buff.empty())
	{
		std::cerr << BGREEN "[Server] " <<  GREEN "There's no message pending to be sent.\n" DEFAULT;
		return FALSE;
	}
	res = send(fd, buff.c_str(), buff.length(), 0);
	if (res == ERROR)
	{
		std::cerr << BGREEN "[Server] " <<  GREEN " Sending reply failed.\n" DEFAULT;
		resetRecvMsg();
		return FALSE;
	}
	if (res == 0)
	{
		this->setDeconnStatus(true);
		resetRecvMsg();
		return FALSE;
	}
	std::cout << BGREEN "[Server] " <<  GREEN "Message to client " << this->getFd() << DEFAULT " >>  "<< buff;
	resetRecvMsg();
	return TRUE;
}

bool				Client::isRegistred(void)
{
	if (this->_state._registred == TRUE)
		return TRUE;
	if (!_infos._nickname.empty() && !_infos._username.empty() && !_infos._pwd.empty() && !_infos._realname.empty())
	{
		if (_state._registred == FALSE)
			_state._registred = TRUE;
		return TRUE;
	}
	return FALSE;	
}

void				Client::welcomeClient(Server *serv)
{
	if (this->isRegistred() == FALSE)
		return ;
	if (getWelcomeStatus() == FALSE)
	{
		addToClientBufferExtended(serv, this->getFd(), RPL_WELCOME(this->getNickname(), this->getPrefix()));
		addToClientBufferExtended(serv, this->getFd(), RPL_YOURHOST(this->getNickname(), this->_server->getServerName(), "0.1"));
		addToClientBufferExtended(serv, this->getFd(), RPL_CREATED(this->getNickname(), this->_server->getStartTime()));
		addToClientBufferExtended(serv, this->getFd(), RPL_MYINFO(this->getNickname(), this->_server->getServerName(), "1.1", "io", "kost"));
		
		addToClientBufferExtended(serv, this->getFd(), RPL_MOTDSTART(this->getNickname(), "42_Ft Irc (localhost)"));
		addToClientBufferExtended(serv, this->getFd(), RPL_MOTD(this->getNickname(), " :- Welcome to our IRC server!"));
		addToClientBufferExtended(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- .-.----------.-."));
		addToClientBufferExtended(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- | |--FT_IRC--| |"));
		addToClientBufferExtended(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- | |----------| |"));
		addToClientBufferExtended(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- | |-blefebvr-| |"));
		addToClientBufferExtended(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- | |-pmaimait-| |"));
		addToClientBufferExtended(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- | * 42-Paris * |"));
		addToClientBufferExtended(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- |@            @|"));
		addToClientBufferExtended(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- |@ @   @ @  @ @|"));
		addToClientBufferExtended(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- |@   @     @  @|"));
		addToClientBufferExtended(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- |-^^---^-^^--^-|"));
		addToClientBufferExtended(serv, this->getFd(), RPL_MOTD(this->getNickname(), "- .-.----------.-."));
		addToClientBufferExtended(serv, this->getFd(), RPL_ENDOFMOTD(this->getNickname()));
		_state._welcomed = TRUE;
	}
}

void Client::partAllChannel(void)
{
    if (_channel.empty())
    {
        _channel.clear();
        return;
    }
    for (size_t i = _channel.size(); i > 0; --i)
    {
        Channel *channel = _channel[i - 1]; // Access to th element at index i - 1
        if (channel)
            channel->partChannel(this, "");
    }
    _channel.clear();
}
