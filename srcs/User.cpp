#include "User.hpp"
#include <iostream>

User::User(TCP::TCPSocket *socket) : 
	BasicConnection(socket),
	_registered(false),
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


bool                    User::isRegistered() const { return (_registered); }

int                     User::state() const { return (_state); }
void					User::setState(int state) { _state = state; }


int                     User::reply(User &u, int code, std::vector<std::string> args) {
	u.write(getResponse(u, code, args));
	return (0);
}

std::string             User::getResponse(User &u, int code, std::vector<std::string> args) {
	std::string target;
	if (u.state() == 0 || u.state() == 1)
		target = "*";
	else
		target = u.nickname();
	target += " ";
	switch (code) {
		case 431:
			return target + ":No nickname given";
		case 461:
			return target + ": " + args[0] + ":Not enough parameters";
		case 462:
			return target + ":You may not reregister";
		case 464:
			return target + ":Password incorrect";
		default:
			return std::string(args[0]);
	}
}
