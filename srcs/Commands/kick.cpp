#include "IrcServer.hpp"

int		IrcServer::KICK(User &u, Message msg) {
	if (u.state() != 2)
		return (u.reply(u, ERR_NOTREGISTERED(u.nickname())));
	if (msg.args().size() < 2)
		return (u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), "KICK")));
	std::string channel = msg.args()[0];
	std::string target = msg.args()[1];

	if (channel[0] != '#') {
		return u.reply(u, ERR_NOSUCHCHANNEL(u.nickname(), channel));
	}
	Channel *c = _network.getChannel(channel);
	if (!c)
		return u.reply(u, ERR_NOSUCHCHANNEL(u.nickname(), channel));
	UserMode *um = c->getUser(&u);
	if (!um)
		return u.reply(u, ERR_NOTONCHANNEL(u.nickname(), channel));
	if (!um->isChanOP())
		return u.reply(u, ERR_CHANOPRIVSNEEDED(u.nickname(), channel));
	User *t = _network.getByNickname(target);
	if (!t)
		return u.reply(u, ERR_NOSUCHNICK(u.nickname(), target));
	c->broadcast(&u, "KICK " + channel + " " + target + " :" + (msg.args().size() > 2 ? msg.args()[2] : "(no reason)"));
	c->delUser(t);
	if (c->clientSize() == 0)
		_network.remove(c);
	return (1);
}