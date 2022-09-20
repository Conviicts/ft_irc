#include "User.hpp"
#include <iostream>

User::User(TCP::TCPSocket *socket) :
	BasicConnection(socket),
	_channel(NULL),
	_registered(false),
	_mode(false),
	_state(0),
	_idle(time(NULL))
{}

User::~User() {}

const std::string		&User::nickname() const { return (_nickname); }
void 					User::setNickname(const std::string &nickname) { _nickname = nickname; }


const std::string		&User::username() const { return (_username); }
void 					User::setUsername(const std::string &username) { _username = username; }

const std::string		&User::realname() const { return (_realname); }
void 					User::setRealname(const std::string &realname) { _realname = realname; }


bool					User::isRegistered() const { return (_registered); }

bool					User::isOperator() const { return (_mode); }
void					User::setOperator(bool value) { _mode = value; }

std::string 			User::getPrefix() const {
	return _nickname + (_username.empty() ? "" : "!" + _username) + (socket()->host().empty() ? "" : "@" + socket()->host());
}

int						User::state() const { return (_state); }
void					User::setState(int state) { _state = state; }


int						User::reply(User &u, std::string const &reponse) {
	u.write(getResponse(u, reponse));
	return (0);
}

void					User::welcome(User &u) {
	std::cout << _state << " " << _username << " " << _nickname << " " << _realname << std::endl;
	if (_state != 1 || _username.empty() || _realname.empty() || _nickname.empty())
		return ;
	_state = 2;
	reply(u, RPL_WELCOME(_nickname));
	std::cout << u.socket()->host() << " is not known as " << _nickname << std::endl;
}

std::string				User::getResponse(User &u, std::string const &reponse) {
	std::string target;
	if (u.state() == 0 || u.state() == 1)
		target = "*";
	else
		target = u.getPrefix();
	target += " ";
	return reponse;
}

Channel					*User::getChannel() const { return _channel; };

void					User::joinChannel(User &u, Channel *channel) {
	channel->addUser(this);
	_channel = channel;

	channel->broadcast(":" + u.nickname() + " JOIN :" + channel->name());
	std::cout << u.nickname() << " JOIN :" << channel->name() << std::endl;
}

void					User::setChannel(Channel *channel) { _channel = channel; }