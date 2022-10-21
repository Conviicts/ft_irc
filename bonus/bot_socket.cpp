#include "includes/bot_socket.hpp"
#include <iostream>

bot::bot() : _buffer_rcv(4096), _buffer_snd(4096), _fd(-1) {}
bot::~bot() {}

int		error(int error_type)
{
	if (error_type == 1)
		std::cout << "Error socket fd" << std::endl;
	if (error_type == 2)
		std::cout << "Error unknowed hostname or wrong port" << std::endl;
	if (error_type == 3)
		std::cout << "Error connection to host" << std::endl;
	return (-1);
}

int	bot::fd(void) const
{
	return (_fd);
}

int	bot::cnct(const char *hostname, uint16_t port)
{
	struct sockaddr_in saddr;
	struct hostent *h;

	if ((_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		return (error(1));
	if (!(h = gethostbyname(hostname)))
		return (error(2));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr = *((struct in_addr*)h->h_addr);
	if (connect(_fd ,(struct sockaddr*)&saddr, sizeof(saddr)) == -1)
		return (error(3));
	return (1);
}

int	bot::dscnct(void)
{
	return (close(_fd));
}

int bot::in(void)
{
	int ret;
	_buffer_rcv.assign(2096, 0x00);
	if ((ret = recv(_fd, &_buffer_rcv[0], _buffer_rcv.size(), 0)) == -1)
		if (errno != EWOULDBLOCK)
			throw std::runtime_error("recv() failed");
	if (ret)
		_rcv.assign(_buffer_rcv.begin(), _buffer_rcv.end());
	std::cout << "recv =>" << _rcv << std::endl;
	return (ret);
}

int bot::out(void)
{
	int ret;
	if ((ret = send(_fd, &_buffer_snd[0], _buffer_snd.size(), 0)) == -1)
		if (errno != EWOULDBLOCK)
			throw std::runtime_error("send() failed");
	_snd.assign(_buffer_snd.begin(), _buffer_snd.end());
	_buffer_snd.clear();
	std::cout << "send =>" << _snd << std::endl;
	return (ret);
}

bot & bot::operator<<(const char *buffer)
{
	for (size_t i = 0; i < strlen(buffer); i++)
	{  
		_buffer_snd.push_back(buffer[i]);
		if (_buffer_snd[_buffer_snd.size() - 1] == '\n' && _buffer_snd[_buffer_snd.size() - 2] == '\r')
			out();
	}
	return (*this);
}