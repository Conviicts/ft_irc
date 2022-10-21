#include "IrcServer.hpp"
#include "Replies.hpp"

std::vector<std::string>    split(const std::string &str, char delim);

int		IrcServer::PRIVMSG(User &u, Message msg) {
	if (u.state() != 2)
		return (u.reply(u, ERR_NOTREGISTERED(u.nickname())));
	if (msg.args().size() < 2)
		return (u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), "PRIVMSG")));
	std::string message;

	for (std::vector<std::string>::const_iterator it = msg.args().begin() + 1; it != msg.args().end(); it++)
		message.append(*it + " ");
	message = message.at(0) == ':' ? message.substr(1) : message;
	message = message.at(message.size() - 1) == ' ' ? message.substr(0, message.size() - 1) : message;
	
	std::vector<std::string> targets = split(msg.args()[0], ',');
	for (std::vector<std::string>::const_iterator it = targets.begin(); it != targets.end(); it++) {
		std::string target(*it);
		if (target.at(0) == '#') {
			Channel *c = _network.getChannel(target);
			if (!c)
				return u.reply(u, ERR_NOSUCHCHANNEL(u.nickname(), target));
			if (!c->getUser(&u))
				return u.reply(u, ERR_NOTONCHANNEL(u.nickname(), target));
			c->broadcast(&u, ":" + u.getPrefix() + " PRIVMSG " + target + " :" + message);
		} else {
			User *t = _network.getByNickname(target);
			if (!t)
				return u.reply(u, ERR_NOSUCHNICK(u.nickname(), target));
			t->write(":" + u.getPrefix() + " PRIVMSG " + target + " :" + message);
		}
	}
	return (1);
}