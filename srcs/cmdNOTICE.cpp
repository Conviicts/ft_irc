#include "IrcServer.hpp"

int IrcServer::NOTICE(User &u, Message msg) {
    (void)u;
    (void)msg;

    std::cout << "TODO: NOTICE" << std::endl; 
    if (msg.args().size() < 2 || msg.args()[0].empty() || msg.args()[1].empty())
        return (0);
    std::string target = msg.args()[0];
    std::string message;

    for (std::vector<std::string>::const_iterator it = msg.args().begin() + 1; it != msg.args().end(); ++it)
        message.append(*it + " ");
    message = message.at(0) == ':' ? message.substr(1) : message;
    if (target.at(0) == '#') {
        Channel *channel = u.getChannel();
        if (!channel)
            return (0);
        
    }
    return (1);
}