#include "IrcServer.hpp"

int		IrcServer::NICK(User &u, Message msg) {

	if (!msg.args().size() || msg.args()[0].empty())
		return u.reply(u, ERR_NONICKNAMEGIVEN(u.nickname()));
	if (_network.getByNickname(msg.args()[0]))
		return u.reply(u, ERR_NICKNAMEINUSE(u.nickname()));
	_network.remove(&u);
	u.setNickname(msg.args()[0]);
	_network.add(&u);
	return (1);
}