#include "IrcServer.hpp"

// IL FAUT CHECK LE MODE SUR INVITATION DU CHANNEL
// ET DANS CE CAS SEUL LES USER INVITED PEUVENT JOIN
// BISOUS 😘

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
		c = new Channel(msg.args()[0], (!msg.args()[1].empty()) ? msg.args()[1] : "", &u);
		_network.add(c);
		newChan = 1;
		std::cout << "New channel created: " << msg.args()[0] << std::endl;
	} else if (c->getUser(&u)) {
		return 0;
	} else {
		bool isInvited = c->isInvited(u);

		if (!isInvited && c->isInviteOnly()) {
			return u.reply(u, ERR_INVITEONLYCHAN(u.nickname(), c->name()));
		}
		std::cout << c->password() << std::endl;
		if (!c->password().empty()) {
			if (msg.args().size() < 2 || msg.args()[1].empty() || c->password() != msg.args()[1])
				return u.reply(u, ERR_BADCHANNELKEY(u.nickname(), c->name()));
		}
	}
	c->addUser(&u, newChan == 1 ? UserMode(UserMode::CREATOR) : UserMode(UserMode::USER));
	u.joinChannel(u, c);
	return (1);
}