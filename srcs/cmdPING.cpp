#include "IrcServer.hpp"

int		IrcServer::PING(User &u, Message msg) {
	if (msg.args().size() == 0)
		return u.reply(u, ERR_NOORIGIN(u.nickname()));
	u.reply(u, RPL_PONG(u.nickname(), msg.args()[0]));
	return (1);
}