#include "Channel.hpp"
#include <algorithm>

Channel::Channel(const std::string &name, const std::string &password, User *admin) :
	_maxUsers(99),
	_type(name.c_str()[0] == '#' ? 0 : 1),
	_invite_only(false),
	_admin(admin),
	_name(name),
	_password(password)
{

}

Channel::~Channel() { }

const std::string			&Channel::name() const { return _name; }

int							Channel::type() const { return _type; }
void						Channel::setType(int type) { _type = type; }

const std::string			&Channel::password() const { return _password; }

void						Channel::setPassword(const std::string &password) { _password = password; }

void						Channel::addUser(User *user, const UserMode &mode) {
	_users[user] = mode;
	user->setChannelsCount(user->channelsCount() + 1);
	std::set<std::string>::const_iterator it = _invited.find(user->nickname());
	if (it != _invited.end())
		_invited.erase(it);
}

void						Channel::delUser(User *user) {
	_users.erase(user);
	user->setChannelsCount(user->channelsCount() - 1);
}

int							Channel::maxUsers() const { return _maxUsers; }
void						Channel::setMaxUsers(int maxUsers) { _maxUsers = maxUsers; }

const std::string			&Channel::getTopic() const { return _topic; }
void						Channel::setTopic(std::string const & topic) { _topic = topic; }

void						Channel::broadcast(User *user, const std::string &msg) {
	for (std::map<User *, UserMode>::iterator it = _users.begin(); it != _users.end(); ++it) {
		User *u(it->first);
		if (u->nickname() == user->nickname())
			continue;
		u->write(msg);
	}
}

void						Channel::broadcast2(const std::string &msg) {
	for (std::map<User *, UserMode>::iterator it = _users.begin(); it != _users.end(); ++it) {
		User *u(it->first);
		u->write(msg);
	}
}

void						Channel::kick(User *user, User *target, const std::string &reason) {
	broadcast(user, ":" + user->nickname() + " KICK " + _name + " " + target->nickname() + " :" + reason);
	delUser(target);
	// std::cout << user->nickname() << "kicked" << target->nickname() << "from channel " << _name << std::endl;
}

int							Channel::clientSize() const { return _users.size(); }

std::vector<std::string>	Channel::usersNick() {
	std::vector<std::string> nicks;

	for (std::map<User *, UserMode>::const_iterator it = _users.begin(); it != _users.end(); it++) {
		if ((*it).second.isChanOP())
			nicks.push_back("@" + it->first->nickname());
		else
			nicks.push_back(it->first->nickname());
	}
	return nicks;
}

UserMode 					*Channel::getUser(User *user) {
	std::map<User *, UserMode>::iterator it(_users.find(user));
	return (it != _users.end() ? &it->second : NULL);
}

UserMode 					*Channel::getUserByNickname(std::string nickname) {
	for (std::map<User *, UserMode>::iterator it = _users.begin(); it != _users.end(); ++it) {
		if (it->first->nickname() == nickname)
			return &it->second;
	}
	return NULL;
}


void						Channel::setInviteOnly(bool mode) { _invite_only = mode; }

void						Channel::invite(User *user) { _invited.insert(user->nickname()); }

bool						Channel::isInvited(User const &user) const {
	return _invited.find(user.nickname()) != _invited.end();
}

bool						Channel::isInviteOnly() const { return _invite_only; }

std::map<User *, UserMode>	Channel::users() const {
	return _users.empty() ? std::map<User *, UserMode>() : _users;
}
