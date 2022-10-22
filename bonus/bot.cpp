#include <iostream>
#include <signal.h>
#include "includes/bot_socket.hpp"

bot 	my_bot;

void help(char *fname);
void quit(int sig);

std::vector<std::string>    split(const std::string &str, char delim) 
{
	std::vector<std::string>    res;
	std::string                 tmp;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == delim) 
		{
            res.push_back(tmp);
            tmp.clear();
        }
		else
            tmp += str[i];
    }
    res.push_back(tmp);
    return (res);
}

int random(int min, int max)
{
	static bool first = true;
	if (first)
	{  
		srand(time(NULL));
		first = false;
	}
	return (min + rand() % (( max + 1 ) - min));
}


int	bot::client_on(const char *hostname, uint16_t port, const char *nick)
{
	if (cnct(hostname,port) == -1)
		return (-1);
	*this << "PASS test" << "\r\n";
	*this << "NICK " << nick << "\r\n";
   	*this << "USER " << nick <<  " " << nick <<  " " << nick << " " << nick << "\r\n";
	return (1);
}

void	bot::game(const char *channel)
{
	int number;

	number = random(1, 200);
	std::cout << "number: " << number << std::endl;
	while(in())
	{
		std::string tmp = split(_rcv, ':')[2];
		if (strlen(tmp.c_str()) > 9 || tmp.find_first_not_of("0123456789") == std::string::npos)
			*this << "PRIVMSG " << "#" << channel << " :Please choose a integer!\r\n";
		else
		{
			int ret = std::atoi(tmp.c_str());
			if (ret > number)
        	    *this << "PRIVMSG " << "#" << channel << " :less!\r\n";
			else if (ret < number)
				*this << "PRIVMSG " << "#" << channel << " :more\r\n";
			else
			{
				*this << "PRIVMSG " << "#" << channel << " :You win :D!\r\n";
				break;
			}
		}
	}
}

void	bot::scan(char **argv)
{
	size_t n;

	while (in()) 
	{
        if (_rcv.compare(0, 6, "PING ") == 0)
            *this << "PO \n\r";
    	else if (_rcv.compare(0, 4, "001 ") == 0)
		{
				*this << "OPER " << "admin" << " " << "admin" << "\r\n";
            	*this << "MODE " << argv[4] << " +o\r\n";
            	*this << "JOIN " << "#" << argv[3] << "\r\n";
        }
		n = _rcv.find("PRIVMSG #");
		if (n != std::string::npos)
		{
			std::string 	tmp("!");

			tmp += argv[4];
			std::cout << "tmp =>" << tmp << std::endl;
			if (_rcv.find(tmp) != std::string::npos)
			{
            	*this << "PRIVMSG " << "#" << argv[3] << " :Someone just spoke to me!\r\n";
				*this << "PRIVMSG " << "#" << argv[3] << " :Game 'Le juste prix' start\r\n";
				*this << "PRIVMSG " << "#" << argv[3] << " :Give a number, I'll tell you: more or less! have fun ;D\r\n";
				game(argv[3]);
			}
		}
    }
}

int main(int argc, char** argv)
{
    if (argc < 5)
		help(argv[0]);
    struct sigaction sa;
    sa.sa_handler = &quit;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT,&sa,0);
	if (strlen(argv[2]) > 9)
	{
		std::cout << "Check port please !" << std::endl;
		return (1);
	}
    if (my_bot.client_on(argv[1],atoi(argv[2]), argv[4]) == -1)
		return (1);
	my_bot.scan(argv);
	my_bot.dscnct();
    return (0);
}

void help(char *fname)
{
    std::cout << "Usage: " << fname << " hostname port channel nick" << std::endl;
    exit(1);
}

void quit(int sig)
{
	(void)sig;
    my_bot << "QUIT :Pressed CTRL-C Good-Bye !\r\n";
    my_bot.dscnct();
    exit(0);
}