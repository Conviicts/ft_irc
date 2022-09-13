#include "Channel.hpp"

Channel::Channel(const std::string &name, const std::string &password, User *admin) :
    _name(name),
    _password(password),
    _admin(admin) {

}

Channel::~Channel() { }


User                *Channel::admin() const { return _admin; }

const std::string   &Channel::name() const { return _name; }

const std::string   &Channel::password() const { return _password; }

void                Channel::setPassword(const std::string &password) { _password = password; }

void                Channel::addUser(User *user) { _users.push_back(user); }

void                Channel::broadcast(const std::string &msg) {
    for (std::vector<User *>::iterator it = _users.begin(); it != _users.end(); ++it)
        (*it)->write(msg);
}
