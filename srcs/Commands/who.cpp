#include "IrcServer.hpp"

int		IrcServer::WHO(User &u, Message msg) {

	if (u.state() != 2)
		return (u.reply(u, ERR_NOTREGISTERED(u.nickname())));
	if (msg.args().size() < 1)
		return (u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), "WHO")));
	std::string target = msg.args()[0];
	if (target.at(0) == '#') {
		Channel *c = _network.getChannel(target);
		if (!c)
			return u.reply(u, ERR_NOSUCHCHANNEL(u.nickname(), target));
		for (std::map<User *, UserMode>::const_iterator it = c->_users.begin(); it != c->_users.end(); it++) {
			if (msg.args().size() > 1 && msg.args()[1] == "o") {
				if (it->second.isChanOP())
					u.reply(u, RPL_WHOREPLY(u.nickname(), c->name(), it->first->username(), it->first->socket()->host(), "ft_irc", it->first->nickname(), "H", "0", it->first->realname()));
			} else {
				u.reply(u, RPL_WHOREPLY(u.nickname(), c->name(), it->first->username(), it->first->socket()->host(), "ft_irc", it->first->nickname(), "H", "0", it->first->realname()));
			}
		}
		u.reply(u, RPL_ENDOFWHO(u.nickname()));
	} else {
		User *t = _network.getByNickname(target);
		if (!t)
			return u.reply(u, ERR_NOSUCHNICK(u.nickname(), target));
		u.reply(u, RPL_WHOREPLY(u.nickname(), "*", t->username(), t->socket()->host(), "ft_irc", t->nickname(), "H", "0", t->realname()));
		u.reply(u, RPL_ENDOFWHO(u.nickname()));
	}
	return (1);
}