#include "IrcServer.hpp"

int     IrcServer::PRIVMSG(User &u, Message msg) {

    if (msg.args().size() < 2)
        return u.reply(u, 461, msg.args());
    std::string target = msg.args()[0];
    std::string message;

    for (std::vector<std::string>::const_iterator it = msg.args().begin() + 1; it != msg.args().end(); it++)
		message.append(*it + " ");
	message = message.at(0) == ':' ? message.substr(1) : message;
    //TODO: can join channel without # ?
    if (target.at(0) == '#') {
        Channel *channel = u.getChannel();
        if (!channel) {
            u.reply(u, 403, msg.args());
            return (0);
        }
        //TODO: don't receive your own message
        channel->broadcast(": " + u.username() + " PRIVMSG " + target + " :" + message);
        return (1);
    }
    User *msg_target = _network.getByNickname(target);
    std::cout << _network.getByNickname(target) << std::endl;
    if (!msg_target)
        return u.reply(u, 401, msg.args());
    msg_target->write(":" + u.nickname() + " PRIVMSG " + target + " :" + message);
    return (1);
}