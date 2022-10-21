#include "IrcServer.hpp"

// Si un JOIN a lieu avec succès, on envoie à l'utilisateur le sujet du canal (en utilisant RPL_TOPIC)
// et la liste des utilisateurs du canal (en utilisant RPL_NAMREPLY), y compris lui-même.

std::vector<std::string>	split(const std::string &str, char delim);

int		IrcServer::JOIN(User &u, Message msg) {
	if (u.state() != 2)
		return (u.reply(u, ERR_NOTREGISTERED(u.nickname())));
	if (!msg.args().size())
		return (u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), "JOIN")));
	
	if (msg.args().size() < 1)
		return u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), msg.args()[0]));
	
	if (msg.args()[0][0] != '#') {
		u.reply(u, ERR_NOSUCHCHANNEL(u.nickname(), msg.args()[0]));
		return 0;
	}
	std::vector<std::string> channels = split(msg.args()[0], ',');
	std::vector<std::string> passwords;
	if (msg.args().size() > 1)
		passwords = split(msg.args()[1], ',');
	else
		passwords = std::vector<std::string>();
	std::vector<std::string>::iterator itpass = passwords.begin();
	for (std::vector<std::string>::const_iterator it = channels.begin(); it != channels.end(); it++) {
		Channel *c = _network.getChannel(*it);
		bool newChan = 0;
		if (!c) {
			c = new Channel(*it, (msg.args().size() > 1 && !(*itpass).empty()) ? (*itpass) : "", &u);
			_network.add(c);
			newChan = 1;
		} else if (c->getUser(&u)) {
			std::cout << "User already in channel" << std::endl;
			continue ;
		} else {
			bool isInvited = c->isInvited(u);

	 		if ((int)(c->_users.size() + 1) > (int)c->maxUsers()) {
				u.reply(u, ERR_CHANNELISFULL(u.nickname(), c->name()));
				continue ;
			}
			if (!isInvited && c->isInviteOnly()) {
				u.reply(u, ERR_INVITEONLYCHAN(u.nickname(), c->name()));
				continue ;
			}
			if (!c->password().empty()) {
				if (msg.args().size() < 2 || (*itpass).empty() || c->password() != (*itpass)) {
					u.reply(u, ERR_BADCHANNELKEY(u.nickname(), c->name()));
					continue ;
				}
			}
		}
		c->addUser(&u, newChan == 1 ? UserMode(UserMode::CREATOR) : UserMode(UserMode::USER));
		u.joinChannel(u, c, newChan);
		itpass++;
	}
	return (1);
}