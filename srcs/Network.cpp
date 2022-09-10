#include "Network.hpp"
#include <iostream>

Network::Network() { }

Network::~Network() throw() { clear(); }

void				        Network::add(User *user) {
    _connections[user->socket()] = user;
}

void				        Network::remove(User *user) throw() {
	if (!user->count())
		_connections.erase(user->socket());
	_users.erase(user->nickname());
}

void                        Network::clear() throw() {

}

TCP::BasicConnection		*Network::getUserBySocket(TCP::TCPSocket *socket) {
    Connection::const_iterator i = _connections.find(socket);
	return (i == _connections.end() ? NULL : i->second);
}

const Network::Connection  	&Network::connections() const {
    return _connections;
}

const Network::Users  		&Network::users() const {
    return _users;
}

void Network::newZombie(TCP::BasicConnection *z) {
	_zombies.push_back(z);
}

TCP::BasicConnection        *Network::nextZombie() {
	if (_zombies.empty())
		return NULL;
	TCP::BasicConnection *z = _zombies.front();
	_zombies.pop_front();
	return z;
}