#pragma once
#include <iostream>

#include "TCPServer.hpp"
#include "User.hpp"
#include "Network.hpp"
#include "Message.hpp"

class IrcServer {
    public:
        IrcServer();
        ~IrcServer();

        enum State {
            STARTED,
            RESTARTING,
            CLOSED
        };

        int             PASS(User &u, Message msg);
        int             NICK(User &u, Message msg);
        int             USER(User &u, Message msg);
        int             QUIT(User &u, Message msg);

        void            listen(const std::string &port, size_t maxQueueLen = 5);
        void            run(); // Run the server
        
        void            flushZombies(); // Flush the zombies

        State           state() const; // get the server state
    private:
	    typedef int (IrcServer::*UserCommandPointer)(User &, Message);
	    typedef std::map<std::string, UserCommandPointer> userCommands;

        State           _state; // Current state of the server
        bool            _init; // True if the server is initialized
        TCP::TCPServer  _server; // The tcp server
        Network			_network;

        userCommands	_userCommands;


        void            execute(TCP::BasicConnection *c, Message message);

        void            disconnect(TCP::TCPSocket *socket, const std::string &reason) throw(); // Disconnect a client
	    void            disconnect(User &u, const std::string &reason, bool notifyUser = 0) throw(); // Disconnect a client
        
        IrcServer(const IrcServer &ref);
        IrcServer &operator=(const IrcServer &ref);
};