#include "IrcServer.hpp"

int		IrcServer::USER(User &u, Message msg) {
	if (msg.args().size() != 4)
		return u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), msg.command()));
	if (u.state() == 2)
		return u.reply(u, ERR_ALREADYREGISTRED(u.nickname()));
	
	u.setUsername(msg.args()[0]);
	u.setHostname(msg.args()[1]);
	u.setServerName(msg.args()[2]);
	u.setRealname(msg.args()[3]);
	u.welcome(u);

	return (1);
}