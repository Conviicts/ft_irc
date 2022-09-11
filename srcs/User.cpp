#include "User.hpp"

User::User(TCP::TCPSocket *socket) : 
	BasicConnection(socket),
	_registered(false),
	_state(0),
	_idle(time(NULL))
{}

User::~User() {}

const std::string		&User::nickname() const { return (_nickname); }

void 					User::setNickname(const std::string &nickname) { _nickname = nickname; }

bool                    User::isRegistered() const { return (_registered); }

int                     User::state() const { return (_state); }
void					User::setState(int state) { _state = state; }
