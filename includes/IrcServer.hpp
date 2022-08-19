#pragma once
#include <iostream>

#include "TcpServer.hpp"

class IrcServer {
    public:
        IrcServer();
        ~IrcServer();

        enum State {
            STARTED,
            RESTARTING,
            CLOSED
        };

        void            listen(const std::string &port, size_t maxQueueLen = 5);
        void            run(); // Run the server
        
        State           state() const; // get the server state
    private:
        State           _state; // Current state of the server
        bool            _init; // True if the server is initialized
        TCP::TcpServer  _server; // The tcp server


        void            disconnect(TCP::TcpSocket *socket, const std::string &reason) throw(); // Disconnect a client
        
        IrcServer(const IrcServer &ref);
        IrcServer &operator=(const IrcServer &ref);

};