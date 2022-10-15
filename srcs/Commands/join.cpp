#include "IrcServer.hpp"

// Si un JOIN a lieu avec succès, on envoie à l'utilisateur le sujet du canal (en utilisant RPL_TOPIC)
// et la liste des utilisateurs du canal (en utilisant RPL_NAMREPLY), y compris lui-même.

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
	Channel *c = _network.getChannel(msg.args()[0]);
	bool newChan = 0;
	if (!c) {
		c = new Channel(msg.args()[0], (msg.args().size() > 1 && !msg.args()[1].empty()) ? msg.args()[1] : "", &u);
		_network.add(c);
		newChan = 1;
	} else if (c->getUser(&u)) {
		std::cout << "User already in channel" << std::endl;
		return 0;
	} else {
		bool isInvited = c->isInvited(u);

		if ((int)(c->_users.size() + 1) > (int)c->maxUsers()) {
			u.reply(u, ERR_CHANNELISFULL(u.nickname(), c->name()));
			return 0;
		}
		if (!isInvited && c->isInviteOnly()) {
			return u.reply(u, ERR_INVITEONLYCHAN(u.nickname(), c->name()));
		}
		if (!c->password().empty()) {
			if (msg.args().size() < 2 || msg.args()[1].empty() || c->password() != msg.args()[1])
				return u.reply(u, ERR_BADCHANNELKEY(u.nickname(), c->name()));
		}
	}
	// std::cout << "User " << u.nickname() << " joined channel " << c->name() << std::endl;
	c->addUser(&u, newChan == 1 ? UserMode(UserMode::CREATOR) : UserMode(UserMode::USER));
	u.joinChannel(u, c, newChan);
	
	return (1);
}