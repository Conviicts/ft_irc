#include "IrcServer.hpp"

int     IrcServer::USER(User &u, Message msg) {

    if (u.state() == 0) {
        disconnect(u, "bad password", true);
        return 0;
    }
    if (msg.args().size() != 4)
        return u.reply(u, 461, msg.args());
    if (u.state() != 1)
        return u.reply(u, 462, std::vector<std::string>());
    u.setUsername(msg.args()[0]);
    u.setRealname(msg.args()[3]);
    u.welcome(u);

    return (1);
}