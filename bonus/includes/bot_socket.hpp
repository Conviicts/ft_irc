#ifndef __BOT_SOCKET_HPP__
# define __BOT_SOCKET_HPP__

#include <sys/socket.h>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <netdb.h>
#include <cerrno>

class bot {

	private		:
					std::vector<std::string>	_config;
					std::vector<char>			_buffer_rcv;
					std::vector<char> 			_buffer_snd;
					std::string					_rcv;
					std::string         		_snd;
					int							_fd;
	public		:
												bot();
												~bot();
					int							cnct(const char *hostname, uint16_t port);
					int							client_on(const char *hostname, uint16_t port, const char *nick);
					void						scan(char **argv);
					void						game(const char *channel);
					int							dscnct(void);
					int							fd(void) const;
					int							in(void);
					int							out(void);
					bot	&			 			operator<<(const char *buffer);
};

#endif