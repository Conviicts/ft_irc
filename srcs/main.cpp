#include <iostream>

#include "IrcServer.hpp"

int main() {
    IrcServer::State state = IrcServer::STARTED;

    while (state != IrcServer::CLOSED) {
        try {
            IrcServer server;
            server.run();
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            return (1);
        }
    }
}