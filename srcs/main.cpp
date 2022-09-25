#include <iostream>

#include "IrcServer.hpp"

int main(int ac, char **av) {
    (void)ac;
    if (ac != 3) {
        std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    IrcServer::State state = IrcServer::STARTED;
    while (state != IrcServer::CLOSED) {
        try {
            IrcServer server(av);
            server.run();
            state = server.state();
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            return (1);
        }
    }
}