#include "Network.hpp"
#include <iostream>

Network::Network() { }

Network::~Network() throw() { clear(); }

void				        Network::add(User *user) {
    _connections[user->socket()] = user;
	if (user->nickname().size())
		_users[user->nickname()] = user;
}

void				        Network::remove(User *user) throw() {
	if (!user->count())
		_connections.erase(user->socket());
	_users.erase(user->nickname());
}

void                        Network::clear() throw() {
	for (Users::iterator it = _users.begin(); it != _users.end(); ++it)
		delete it->second;
	for (Zombies::iterator it = _zombies.begin(); it != _zombies.end(); ++it)
		delete *it;
}

TCP::BasicConnection		*Network::getUserBySocket(TCP::TCPSocket *socket) {
    Connection::const_iterator i = _connections.find(socket);
	return (i == _connections.end() ? NULL : i->second);
}

User 						*Network::getByNickname(const std::string &name) {
	Users::const_iterator it = _users.find(name);
	return (it == _users.end() ? NULL : it->second);
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

Channel                 *Network::getChannel(const std::string &name) {
	for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		if ((*it)->name() == name)
			return *it;
	}
	return NULL;
}

Channel                 *Network::createChannel(const std::string &name, const std::string &password, User *admin) {
	Channel *channel = new Channel(name, password, admin);
	_channels.push_back(channel);
	return channel;
}

