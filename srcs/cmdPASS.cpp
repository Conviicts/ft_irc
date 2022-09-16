#include "IrcServer.hpp"

int		IrcServer::PASS(User &u, Message msg) {

	if (u.state() != 0) {
		u.reply(u, ERR_ALREADYREGISTRED(u.nickname()));
		return 0;
	}
	if (msg.args().size() != 1)
		return u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), msg.args()[0]));
	if (msg.args()[0] != _password)
		return u.reply(u, ERR_PASSWDMISMATCH(u.nickname()));
	u.setState(1);
	return (1);
}