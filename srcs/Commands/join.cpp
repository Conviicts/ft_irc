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
	for (std::vector<std::string>::const_iterator it = msg.args().begin(); it != msg.args().end(); ++it) {
		if (it->at(0) != '#') {
			u.reply(u, ERR_NOSUCHCHANNEL(u.nickname(), *it));
			continue ;
		}
		Channel *c = _network.getChannel(*it);
		bool newChan = 0;
		if (!c) {
			c = new Channel(*it, "", &u);
			_network.add(c);
			newChan = 1;
		} else if (c->getUser(&u)) {
			continue ;
		} else {
			bool isInvited = c->isInvited(u);

			if (!isInvited && c->isInviteOnly()) {
				u.reply(u, ERR_INVITEONLYCHAN(c->name()));
				continue ;
			}
			//TODO: check password
		}
		c->addUser(&u, newChan == 1 ? UserMode(UserMode::CREATOR) : UserMode(UserMode::USER));
		u.joinChannel(u, c);
	}
	return (1);
}