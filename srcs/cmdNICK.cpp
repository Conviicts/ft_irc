#include "IrcServer.hpp"

int     IrcServer::NICK(User &u, Message msg) {

    if (!msg.args().size() || msg.args()[0].empty())
        return u.reply(u, 431, msg.args());
    if (_network.getByNickname(msg.args()[0]))
		return u.reply(u, 433, msg.args());
    _network.remove(&u);
    u.setNickname(msg.args()[0]);
	_network.add(&u);
    return (1);
}