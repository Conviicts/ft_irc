#include "User.hpp"
#include <iostream>

User::User(TCP::TCPSocket *socket) : 
	BasicConnection(socket),
	_registered(false),
	_state(0),
	_mode(false),
	_idle(time(NULL))
{}

User::~User() {}

const std::string		&User::nickname() const { return (_nickname); }
void 					User::setNickname(const std::string &nickname) { _nickname = nickname; }


const std::string		&User::username() const { return (_username); }
void 					User::setUsername(const std::string &username) { _username = username; }

const std::string		&User::realname() const { return (_realname); }
void 					User::setRealname(const std::string &realname) { _realname = realname; }


bool                    User::isRegistered() const { return (_registered); }

bool                    User::isOperator() const { return (_mode); }
void					User::setOperator(bool value) { _mode = value; }

std::string 			User::getPrefix() const {
	return _nickname + (_username.empty() ? "" : "!" + _username) + (socket()->host().empty() ? "" : "@" + socket()->host());
}

int                     User::state() const { return (_state); }
void					User::setState(int state) { _state = state; }


int                     User::reply(User &u, int code, std::vector<std::string> args) {
	u.write(getResponse(u, code, args));
	return (0);
}

void                    User::welcome(User &u) {
	std::cout << _state << " " << _username << " " << _nickname << " " << _realname << std::endl;
	if (_state != 1 || _username.empty() || _realname.empty() || _nickname.empty())
		return ;
	_state = 2;
	reply(u, 001, std::vector<std::string>());
	std::cout << u.socket()->host() << " is not known as " << _nickname << std::endl;
}

std::string             User::getResponse(User &u, int code, std::vector<std::string> args) {
	std::string target;
	if (u.state() == 0 || u.state() == 1)
		target = "*";
	else
		target = u.getPrefix();
	target += " ";
	switch (code) {
		case 001:
		return RPL_WELCOME(target);
			// return target + ":Welcome " + target + " to the ft_irc network";
		case 401:
			return ERR_NOSUCHNICK(target, args[0]);
			// return target + ": " + args[0] + ":No such nick/channel";
		case 403:
			return ERR_NOSUCHCHANNEL(target, args[0]);
		case 405:
			return target + ":You have joined too many channels";
		case 431:
			return ERR_NONICKNAMEGIVEN(target);
		case 433:
			return ERR_NICKNAMEINUSE(target);
		case 461:
			return ERR_NEEDMOREPARAMS(target, args[0]);
		case 462:
			return ERR_ALREADYREGISTRED(target);
		case 464:
			return ERR_PASSWDMISMATCH(target);
		case 471:
			return ERR_CHANNELISFULL(target);
		case 475:
			return target + ":Cannot join channel (+k)";
		default:
			return std::string("defaul");
	}
}

Channel					*User::getChannel() const { return _channel; };

void                    User::joinChannel(User &u, Channel *channel) {
	channel->addUser(this);
	_channel = channel;

	channel->broadcast(":" + u.nickname() + " JOIN :" + channel->name());
	std::cout << u.nickname() << " JOIN :" << channel->name() << std::endl;
}

void					User::setChannel(Channel *channel) { 
	_channel = channel;
};
