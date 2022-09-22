#include "IrcServer.hpp"
#include "ERR_RPL.hpp"

int		IrcServer::PRIVMSG(User &u, Message msg) {
	if (u.state() != 2)
		return (u.reply(u, ERR_NOTREGISTERED(u.nickname())));
	if (msg.args().size() < 2)
		return (u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), "PRIVMSG")));
	std::string target(msg.args()[0]);
	std::string message;

	for (std::vector<std::string>::const_iterator it = msg.args().begin() + 1; it != msg.args().end(); it++)
		message.append(*it + " ");
	message = message.at(0) == ':' ? message.substr(1) : message;
	if (target.at(0) == '#') {
		Channel *c = _network.getChannel(target);
		if (!c)
			return u.reply(u, ERR_NOSUCHCHANNEL(u.nickname(), msg.args()[0]));
		if (!c->isOnChannel(&u))
			return u.reply(u, ERR_NOTONCHANNEL(u.nickname(), msg.args()[0]));
		c->broadcast(&u, ":" + u.getPrefix() + " PRIVMSG " + target + " :" + message);
	} else {
		User *t = _network.getByNickname(target);
		if (!t)
			return u.reply(u, ERR_NOSUCHNICK(u.nickname(), msg.args()[0]));
		t->write(":" + u.getPrefix() + " PRIVMSG " + target + " :" + message);
	}
	return (1);
}