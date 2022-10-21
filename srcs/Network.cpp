#include "Network.hpp"

Network::Network() { }

Network::~Network() throw() { clear(); }

void						Network::add(User *user) {
	_connections[user->socket()] = user;
	std::cout << user->socket()->ip() << std::endl;
	_users[user->nickname().size() ? user->nickname() : itoa(user->socket()->fd())] = user;
}

void						Network::add(Channel *channel) {
	_channels[channel->name()] = channel;
}

void						Network::remove(User *user) throw() {
	if (!user->count())
		_connections.erase(user->socket());
	_users.erase(user->nickname().size() ? user->nickname() : itoa(user->socket()->fd()));
}

void						Network::remove(const Channel *channel) throw() {
	_channels.erase(channel->name());
	delete channel;
}

void						Network::clear() throw() {
	for (Users::iterator it = _users.begin(); it != _users.end(); ++it)
		delete it->second;
	for (Zombies::iterator it = _zombies.begin(); it != _zombies.end(); ++it)
		delete *it;
	for (Channels::iterator it = _channels.begin(); it != _channels.end(); ++it)
		delete it->second;
}

TCP::BasicConnection		*Network::getUserBySocket(TCP::TCPSocket *socket) {
	Connection::const_iterator i = _connections.find(socket);
	return (i == _connections.end() ? NULL : i->second);
}

User 						*Network::getByNickname(const std::string &name) {
	Users::const_iterator it = _users.find(name);
	return (it == _users.end() ? NULL : it->second);
}

const Network::Connection	&Network::connections() const {
	return _connections;
}

const Network::Users		&Network::users() const {
	return _users;
}

const Network::Channels		&Network::channels() const {
	return _channels;
}

void						Network::newZombie(TCP::BasicConnection *z) {
	_zombies.push_back(z);
}

TCP::BasicConnection		*Network::nextZombie() {
	if (_zombies.empty())
		return NULL;
	TCP::BasicConnection *z = _zombies.front();
	_zombies.pop_front();
	return z;
}

Channel						*Network::getChannel(const std::string &name) {
	Channels::const_iterator i = _channels.find(name);
	return (i != _channels.end() ? i->second : NULL);
}
