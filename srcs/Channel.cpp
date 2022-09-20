#include "Channel.hpp"
#include <algorithm>

Channel::Channel(const std::string &name, const std::string &password, User *admin) :
	_name(name),
	_password(password),
	_admin(admin),
	_invite_only(false) {

}

Channel::~Channel() { }

User						*Channel::admin() const { return _admin; }

const std::string			&Channel::name() const { return _name; }

const std::string			&Channel::password() const { return _password; }

void						Channel::setPassword(const std::string &password) { _password = password; }

void						Channel::addUser(User *user) { _users.push_back(user); }

void						Channel::delUser(User *user) {
	_users.erase(std::remove(_users.begin(), _users.end(), user), _users.end());
	user->setChannel(NULL);
	if (_users.empty())
		return;
	if (_admin == user) {
		_admin = _users.begin()[0];
		std::cout << "Channel: " << _name << " new admin: " << _admin->nickname() << std::endl;
	}
}

int							Channel::maxUsers() const { return _maxUsers; }

void						Channel::setMaxUsers(int maxUsers) { _maxUsers = maxUsers; }

const std::string			&Channel::getTopic() const { return _topic; }
void						Channel::setTopic(std::string const & topic) { _topic = topic; }

void						Channel::broadcast(const std::string &msg) {
	for (std::vector<User *>::iterator it = _users.begin(); it != _users.end(); ++it)
		(*it)->write(msg);
}

void						Channel::kick(User *user, User *target, const std::string &reason) {
	broadcast(":" + user->nickname() + " KICK " + _name + " " + target->nickname() + " :" + reason);
	delUser(target);
	std::cout << user->nickname() << "kicked" << target->nickname() << "from channel " << _name << std::endl;
}

int							Channel::clientSize() const { return _users.size(); }

std::vector<std::string>	Channel::usersNick() {
	std::vector<std::string> nicks;

	for (std::vector<User *>::const_iterator it = _users.begin(); it != _users.end(); it++) {
		User *user = *it;
		nicks.push_back((_admin == user ? "@" : "") + (*it)->nickname());
	}
	return nicks;
}

void Channel::setInviteOnly(bool mode) { _invite_only = mode; }

void Channel::invite(User *user) { _invited.push_back(user); }

void Channel::removeInvited(User &user) {

	for (iterator it = _invited.begin(); it != _invited.end(); ++it)
	{
		if ((*it).nickname() == user.nickname())
		{
			_invited.erase(it);
			return ;
		}
	}
}

bool						Channel::isOnChannel(User const *user) const {

	std::vector<User *>::const_iterator it = _users.begin();

	for (; it != _users.end(); it++) {
		if (*it == user)
			return true;
	}
	return false;
}

bool						Channel::isOperator(User const *user) const {

	std::vector<User*>::const_iterator it = _operator.begin();

	for (; it != _operator.end(); it++) {
		if (*it == user)
			return true;
	}
	return false;
}

bool						Channel::isInvited(User const &user) const {

	std::vector<User*>::const_iterator it = _invited.begin();

	for (; it != _invited.end(); it++) {
		if (*it == &user)
			return true;
	}
	return false;
}

bool						Channel::isInviteOnly() const { return _invite_only; }