#include "IrcServer.hpp"

int     IrcServer::JOIN(User &u, Message msg) {

    if (msg.args().size() < 1)
        return u.reply(u, 461, msg.args());
    std::string password = msg.args().size() > 1 ? msg.args()[1] : "";

    Channel *channel = u.getChannel();
    if (channel) {
        return u.reply(u, 405, msg.args());
        return (0);
    }
    channel = _network.getChannel(msg.args()[0]);
    if (!channel)
        channel = _network.createChannel(msg.args()[0], msg.args()[1], &u);
    if (channel->maxUsers() > 0 && channel->clientSize() >= channel->maxUsers()){
		u.reply(u, 471, msg.args());
		return (0);
	}
    if (channel->password() != password) {
		u.reply(u, 475, msg.args());
		return (0);
	}
    u.joinChannel(u, channel);
    return (1);
}