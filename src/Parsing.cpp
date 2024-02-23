/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 16:33:18 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/19 17:51:21 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Server.hpp"

bool checkArg(std::string port, std::string pwd)
{
	if (pwd.size() > 10)
	{
		std::cerr << pwd << ": chosen password is too long.\n";
		return false;
	}
	for(size_t i = 0; i < pwd.size(); i++)
	{
		if (!std::isprint(pwd[i]))
		{
			std::cerr << pwd << ": chosen password has non-printable character.\n";
			return false;
		}
	}
	for(size_t i = 0; i < port.size(); i++)
	{
		if (!isdigit(port[i]))
		{
			std::cerr << port << ": unvalid port number\n";
			return false;
		}
	}
	if (port.size() > 5)
	{
		std::cerr << port << ": out of port range (0 ~ 65535)\n";
		return false;
	}
	int portNb = atoi(port.c_str());
	if (portNb > 65535 || portNb < 0)
	{
		std::cerr << port << ": out of port range (0 ~ 65535)\n";
		return false;
	}
	return true;
}

std::vector<std::string>	splitMsg(std::string msg, char c)
{
	std::vector<std::string> cmd;
	std::stringstream		parse(msg);
	std::string				line;

	while (getline(parse, line, c))
	{
		cmd.push_back(line);
		line.clear();
	}
	return cmd;
}

bool	Server::isValidNickname(std::string name)
{
	std::map<int, Client *>::iterator it = _clients.begin();
	
	while (it != _clients.end())
	{
		if (it->second->getNickname() == name)
			return FALSE;
		it++;
	}
	return TRUE;
}

bool	Server::isValidChannelName(std::string cName)
{
    cName[0] == '#' ? cName : cName = "#" + cName;
    std::vector<Channel*>::iterator it = _channels.begin();

    while (it != _channels.end())
    {
        if ((*it)->getName() == cName)
            return false;
        ++it;
    } 
    return true;
}

std::string 	parseNickname(std::string str)
{
	std::string name = str;
	size_t i(0);
	
	if (name.size() > 30)
		name.erase(30, name.size() - 30);
	
	while (i < name.size())
	{
		if(std::isalpha(name[i]))
			break ;
		else
		{
			name.erase(i, 1);
			i = -1;
		}
		i++;
	}
	
	for (size_t i = 0; i < name.size(); i++)
	{
		if (!std::isprint(name[i]))
		{
			name.erase(i, 1);
			i = -1;
		}
	}
	return name;
}

std::string parseChannelName(std::string chan)
{
	std::string name = chan;
	
	name[0] == '#' ? name : name.insert(0, 1, '#');
	if (name.size() > 50)
		name.erase(50, name.size() - 50);
	
	while (name.find(',') != std::string::npos)
		name.erase(name.find(','), 1);
		
	for (size_t i = 0; i < name.size(); i++)
	{
		if (!std::isprint(name[i]))
			name.erase(i, 0);
	}
	return name;
}