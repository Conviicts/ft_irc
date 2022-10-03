#include "IrcServer.hpp"

std::vector<std::string>    split(const std::string &str, char delim);

int		IrcServer::KICK(User &u, Message msg) {
	if (u.state() != 2)
		return (u.reply(u, ERR_NOTREGISTERED(u.nickname())));
	if (msg.args().size() < 2)
		return (u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), "KICK")));

	std::vector<std::string> channels = split(msg.args()[0], ',');
	std::vector<std::string> users = split(msg.args()[1], ',');

	for (std::vector<std::string>::const_iterator ita = channels.begin(); ita != channels.end(); ita++) {
		if ((*ita)[0] != '#') {
			u.reply(u, ERR_NOSUCHCHANNEL(u.nickname(), (*ita)));
			continue;
		}
		Channel *c = _network.getChannel(*ita);
		if (!c) {
			u.reply(u, ERR_NOSUCHCHANNEL(u.nickname(), *ita));
			continue;
		}
		UserMode *um = c->getUser(&u);
		if (!um)
			return u.reply(u, ERR_NOTONCHANNEL(u.nickname(), *ita));
		if (!um->isChanOP())
			return u.reply(u, ERR_CHANOPRIVSNEEDED(u.nickname(), *ita));
		for (std::vector<std::string>::const_iterator itb = users.begin(); itb != users.end(); itb++) {
			User *t = _network.getByNickname(*itb);
			if (!t)
				return u.reply(u, ERR_NOSUCHNICK(u.nickname(), *itb));
			c->broadcast(&u, "KICK " + *ita + " " + *itb + " :" + (msg.args().size() > 2 ? msg.args()[2] : "(no reason)"));
			c->delUser(t);
			if (c->clientSize() == 0)
				_network.remove(c);
		}
	}
	return (1);
}