#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>

#include "TCPServer.hpp"
#include "User.hpp"
#include "Network.hpp"
#include "Message.hpp"

class IrcServer {
	public:
		IrcServer(char **av);
		~IrcServer();

		enum State {
			STARTED,
			RESTARTING,
			CLOSED
		};

		int				PASS(User &u, Message msg);
		int				NICK(User &u, Message msg);
		int				USER(User &u, Message msg);
		int				QUIT(User &u, Message msg);
		int				PRIVMSG(User &u, Message msg);
		int				JOIN(User &u, Message msg);
		int				OPER(User &u, Message msg);
		int				MODE(User &u, Message msg);
		int				PART(User &u, Message msg);
		int				TOPIC(User &u, Message msg);
		int				NAMES(User &u, Message msg);
		int				LIST(User &u, Message msg);
		int				INVITE(User &u, Message msg);
		int				KICK(User &u, Message msg);
		int				NOTICE(User &u, Message msg);
		int				WHO(User &u, Message msg);
		int				KILL(User &u, Message msg);
		int				PING(User &u, Message msg);
		int				HELP(User &u, Message msg);

		void			listen(const std::string &port, size_t maxQueueLen = 5);
		void			run(); // Run the server
		
		void			flushZombies(); // Flush the zombies

		State			state() const; // get the server state

		void			logger(int type, const std::string msg, const std::string user, std::vector<std::string> args);

		class UserNotFoundException : public std::exception { // est-ce que je suis capable ?
			public:
				virtual const char * what() const throw() {
					return ("User not found");
				}
		};

		class ChannelNotFoundException : public std::exception { // est-ce que je suis ?
			public:
				virtual const char * what() const throw() {
					return ("Channel not found");
				}
		};

	private:
		typedef int (IrcServer::*UserCommandPointer)(User &, Message);
		typedef std::map<std::string, UserCommandPointer> userCommands;

		std::string		_port;
		std::string		_password;

		State			_state; // Current state of the server
		bool			_init; // True if the server is initialized
		TCP::TCPServer	_server; // The tcp server
		Network			_network;

		userCommands	_userCommands;

		void			execute(TCP::BasicConnection *c, Message message);

		void			disconnect(TCP::TCPSocket *socket, const std::string &reason) throw(); // Disconnect a client
		void			disconnect(User &u, const std::string &reason, bool notifyUser = 0) throw(); // Disconnect a client

		void			_channelMODE(User *u, Message msg, Channel *channel);
		void			_userMODE(User *u, Message msg, User *target);
		
		IrcServer(const IrcServer &ref);
		IrcServer &operator=(const IrcServer &ref);

};