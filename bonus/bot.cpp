#include <iostream>
#include <signal.h>
#include "includes/bot_socket.hpp"

bot 	my_bot;

void help(char *fname);
void quit(int sig);

std::vector<std::string>    split(const std::string &str, char delim) {
	std::vector<std::string>    res;
    std::string                 tmp;
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == delim) {
            res.push_back(tmp);
            tmp.clear();
        } else
            tmp += str[i];
    }
    res.push_back(tmp);
    return res;
}

int random(int min, int max) {
	static bool first = true;
	if (first) {  
		srand( time(NULL) );
		first = false;
	}
	return min + rand() % (( max + 1 ) - min);
}


void	bot::client_on(const char *hostname, uint16_t port, const char *channel, const char *nick)
{
	(void)channel;
	cnct(hostname,port);
	*this << "PASS test" << "\r\n";
	*this << "NICK " << nick << "\r\n";
   	*this << "USER " << nick <<  " " << nick <<  " " << nick << " " << nick << "\r\n";
}

void	bot::game()
{
	int number;
	// size_t n;	

	number = random(1, 200);
	std::cout << "number: " << number << std::endl;
	// n = _rcv.find("PRIVMSG #game :");
	while(in())
	{
		std::string tmp = split(_rcv, ':')[2];
		int ret = std::atoi(tmp.c_str());
		if (ret > number)
            *this << "PRIVMSG " << "#game " << ":less!\r\n";
		else if (ret < number)
			*this << "PRIVMSG " << "#game " << ":more\r\n";
		else
		{
			*this << "PRIVMSG " << "#game " << ":you win\r\n";
			break;
		}
	}
}

void	bot::scan()
{
	size_t n;

	while (in()) 
	{
        if (_rcv.compare(0, 5, "PING ") == 0)
            *this << "PO \n\r";
    	else if (_rcv.compare(0, 4, "001 ") == 0)
		{
				*this << "OPER " << "admin" << " " << "admin" << "\r\n";
            	*this << "MODE " << "bob" << " +o\r\n";
            	*this << "JOIN " << "#game" << "\r\n";
        }
		n = _rcv.find("PRIVMSG #game :");
		if (n != std::string::npos)
		{
			if (_rcv.find("!game") != std::string::npos)
			{
            	*this << "PRIVMSG " << "#game " << ":Someone just spoke to me!\r\n";
				*this << "PRIVMSG " << "#game " << ":Game 'Le juste prix' start\r\n";
				*this << "PRIVMSG " << "#game " << ":Give a number, I'll tell you: more or less! have fun ;D\r\n";
				game();
			}
		}
    }
}

int main(int argc, char** argv)
{
    if (argc<5)
		help(argv[0]);

    struct sigaction sa;
    sa.sa_handler = &quit;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT,&sa,0);
    my_bot.client_on(argv[1],atoi(argv[2]), argv[3], argv[4]);
	my_bot.scan();
	my_bot.dscnct();
    return 0;
}

void help(char *fname) {
    std::cout << "Usage: " << fname << " hostname port channel nick" << std::endl;
    exit(1);
}

void quit(int sig) {
	(void)sig;
    my_bot << "QUIT :Someone pressed CTRL-C on me.\r\n";
    my_bot.dscnct();
    exit(0);
}