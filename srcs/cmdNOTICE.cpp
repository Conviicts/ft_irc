#include "IrcServer.hpp"
#include "ERR_RPL.hpp"

int		IrcServer::NOTICE(User &u, Message msg) {
	(void)u;
	(void)msg;

	if (msg.args().size() < 2 || msg.args()[0].empty() || msg.args()[1].empty())
		return (0);
	std::string target = msg.args()[0];

	/* CHECK fonction message --------------------------------------------------------------------------- */
	std::string message;

	for (std::vector<std::string>::const_iterator it = msg.args().begin() + 1; it != msg.args().end(); ++it)
		message.append(*it + " ");
	message = message.at(0) == ':' ? message.substr(1) : message;
	/* -------------------------------------------------------------------------------------------------- */

	if (target.at(0) == '#') {
		// Channel *channel = u.getChannel();
		// if (!channel)
		// 	return (0);

		// std::vector<std::string>			users(channel->usersNick());
		// std::vector<std::string>::iterator	i;
		
		// for (i = users.begin(); i != users.end(); i++)
		// 	if (*i == u.nickname())
		// 		break;
		// if (i == users.end()) {
		// 	u.reply(u, ERR_CANNOTSENDTOCHAN(u.nickname(), target));
		// 	return (0);
		// }
		// channel->broadcast(RPL_NOTICE(u.getPrefix(), target, message));
		return (1);
	}
	User *dest = _network.getByNickname(target);
	if (!dest)
		return (0);
	dest->write(RPL_NOTICE(u.getPrefix(), target, message));
	return (1);
}