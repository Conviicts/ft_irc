#include "User.hpp"
#include <iostream>

User::User(TCP::TCPSocket *socket) :
	BasicConnection(socket),
	_realname(""),
	_operator(false),
	_state(0),
	_channelsCount(0),
	_idle(time(NULL))
{}

User::~User() {}

const std::string		&User::nickname() const { return (_nickname); }
void 					User::setNickname(const std::string &nickname) { _nickname = nickname; }


const std::string		&User::username() const { return (_username); }
void 					User::setUsername(const std::string &username) { _username = username; }

const std::string		&User::hostname() const { return (_hostName); }
void					User::setHostname(const std::string &hostname) { _hostName = hostname; }

const std::string		&User::serverName() const { return (_serverName); }
void					User::setServerName(const std::string &serverName) { _serverName = serverName; }

const std::string		User::realname() const { return _realname; }
void 					User::setRealname(const std::string &realname) { _realname = realname; }

int						User::channelsCount() { return (_channelsCount); }
void					User::setChannelsCount(const int count) { _channelsCount = count; }

bool					User::isOperator() const { return (_operator); }
void					User::setOperator(bool value) { _operator = value; }

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
	if (_state != 1 || _username.empty() || _realname.empty() || _nickname.empty())
		return ;
	_state = 2;
	reply(u, RPL_WELCOME(_nickname));
	// std::cout << u.socket()->host() << " is not known as " << _nickname << std::endl;
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

void					User::joinChannel(User &u, Channel *channel) {
	std::string users = "";
	std::vector<std::string> usersList = channel->usersNick();
	for (std::vector<std::string>::iterator it = usersList.begin(); it != usersList.end(); it++) {
		users += *it + " ";
	}
	u.reply(u, RPL_NAMREPLY(u.nickname(), channel->name(), users));
	u.reply(u, RPL_ENDOFNAMES(u.nickname(), channel->name()));

	channel->broadcast(&u, ":" + u.nickname() + " JOIN :" + channel->name());
}
