#include "IrcServer.hpp"

int     IrcServer::PASS(User &u, Message msg) {

    if (u.state() != 0) {
        u.reply(u, 462, msg.args());
        return 0;
    }
    if (msg.args().size() != 1)
        return u.reply(u, 461, msg.args());
    if (msg.args()[0] != _password)
        return u.reply(u, 464, msg.args());
    u.setState(1);
    return (1);
}