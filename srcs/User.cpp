#include "User.hpp"

User::User(TCP::TCPSocket *socket) : BasicConnection(socket), _idle(time(NULL)) {}

User::~User() {}

const std::string		&User::nickname() const {
	return (_nickname);
}

void User::setNickname(const std::string &nickname) {
	_nickname = nickname;
}