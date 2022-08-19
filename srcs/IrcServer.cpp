#include "IrcServer.hpp"

IrcServer::IrcServer() : _state(STARTED), _init(false) { }

IrcServer::~IrcServer() { }

IrcServer::State    IrcServer::state() const { return _state; }

void                IrcServer::run() {
    if (!_init) {
        _server.listen("10001");
        _server.setMaxConnections(20);
        _init = true;
    }
    while (_state == STARTED) {
        try {
            _server.select();
        } catch (TCP::TcpServer::SigintException &e) {
            _state = CLOSED;
            return ;
        }
        TCP::TcpSocket *newSocket;
        while ((newSocket = _server.getNewConnection())) {
            //TODO: add new client to the server
        }
        TCP::TcpSocket *socket;
        while ((socket = _server.getPendingConnection()) && _state == STARTED) {
            try {
                socket->flush();
                std::string line;
                while (socket->canRead()) {
                    if (!socket->read(line)) {
                        disconnect(socket, "Remote host closed the connection.");
                        continue ;
                    }
                    if (socket->readBufferSize() > 2048) {
                        disconnect(socket, "Received too much data.");
                        continue ;
                    }
                    if (line.empty())
                        continue ;
                    std::cout << "Received: " << line << std::endl;
                }
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
                disconnect(socket, e.what());
                continue ;
            }
        }
        // pingpong
    }
}

void                IrcServer::disconnect(TCP::TcpSocket *socket, const std::string &reason) throw() {
    (void)socket;
    (void)reason;
    // TODO: disconnect user
}   
