#include "IrcServer.hpp"
#include "ERR_RPL.hpp"

int		IrcServer::NOTICE(User &u, Message msg) {
	(void)u;
	(void)msg;

	if (msg.args().size() < 2)
		return (0);
	std::string target = msg.args()[0];

	/* CHECK fonction message --------------------------------------------------------------------------- */
	std::string message;

	for (std::vector<std::string>::const_iterator it = msg.args().begin() + 1; it != msg.args().end(); ++it)
		message.append(*it + " ");
	message = message.at(0) == ':' ? message.substr(1) : message;
	/* -------------------------------------------------------------------------------------------------- */

	if (target.at(0) == '#') {
		Channel *c = _network.getChannel(target);
		if (c)
			c->broadcast(&u, "NOTICE " + c->name() + " :" + message);
	} else {
		User *t = _network.getByNickname(target);
		if (t)
			t->write("NOTICE " + target + " :" + message);
	}
	return (1);
}