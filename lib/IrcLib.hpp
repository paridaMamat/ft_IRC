/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcLib.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blefebvr <blefebvr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 16:21:43 by blefebvr          #+#    #+#             */
/*   Updated: 2024/02/20 13:38:31 by blefebvr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCLIB_HPP
# define IRCLIB_HPP

/****************** Libraries *******************/

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <limits>
#include <map>
#include <vector>
#include <deque>
#include <fstream>
#include <sstream>
#include <sys/epoll.h>
#include <exception>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <ctime>
#include <sys/types.h>
#include <algorithm>
#include <cctype>
#include <cstdlib>

/****************** Includes *******************/

#include "Replies.hpp"
#include "Command.hpp"

/*********************  Colors *******************/

# define DEFAULT "\001\033[0;39m\002"
# define BRED "\001\033[1;91m\002"
# define RED "\001\033[0;91m\002"
# define YELLOW "\001\033[1;93m\002"
# define BMAGENTA "\001\033[1;95m\002"
# define MAGENTA "\001\033[0;95m\002"
# define GREY "\001\033[1;89m\002"
# define BGREEN "\001\033[1;92m\002"
# define GREEN "\001\033[0;92m\002"
# define BBLUE "\001\033[1;36m\002"
# define BLUE "\001\033[0;36m\002"

/*********************  Macros *******************/

# define ERROR 		-1
# define MAXBUF		4096
# define BACKLOG 	5
# define MAXCONN	10
# define MAXCHAN	10
# define TIMEOUT	-1
# define FALSE		0
# define TRUE		1
# define BREAK		2
# define CONTINUE 	3

#endif