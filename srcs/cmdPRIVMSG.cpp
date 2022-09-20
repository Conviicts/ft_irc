#include "IrcServer.hpp"
#include "ERR_RPL.hpp"

int		IrcServer::PRIVMSG(User &u, Message msg) {

	if (msg.args().size() < 2)
		return u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), msg.args()[0]));
	std::string target = msg.args()[0];
	std::string message;

	for (std::vector<std::string>::const_iterator it = msg.args().begin() + 1; it != msg.args().end(); it++)
		message.append(*it + " ");
	message = message.at(0) == ':' ? message.substr(1) : message;
	//TODO: can join channel without # ?
	if (target.at(0) == '#') {
		Channel *channel = u.getChannel();
		if (!channel) {
			u.reply(u, ERR_NOSUCHCHANNEL(u.nickname(), msg.args()[0]));
			return (0);
		}
		std::vector<std::string>			users(channel->usersNick());
		std::vector<std::string>::iterator	i;
		
		for (i = users.begin(); i != users.end(); i++)
			if (*i == u.nickname())
				break;
		if (i == users.end()) {
			u.reply(u, ERR_CANNOTSENDTOCHAN(u.nickname(), target));
			return (0);
		}
		channel->broadcast(":" + u.getPrefix() + " PRIVMSG " + target + " :" + message);
		return (1);
	}
	User *msg_target = _network.getByNickname(target);
	std::cout << _network.getByNickname(target) << std::endl;
	if (!msg_target)
		return u.reply(u, ERR_NOSUCHNICK(u.nickname(), msg.args()[0]));
	msg_target->write(":" + u.nickname() + " PRIVMSG " + target + " :" + message);
	return (1);
}