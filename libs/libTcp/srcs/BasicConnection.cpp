#include "BasicConnection.hpp"

namespace TCP {
    BasicConnection::BasicConnection(TCPSocket *socket, unsigned count) :
    _socket(socket),
    _count(count),
    _needPong(false),
    _clock(time(NULL)) {}

    BasicConnection::~BasicConnection() { }

    TCPSocket *BasicConnection::socket() const { return (_socket); }

    unsigned BasicConnection::count() const { return (_count); }

    void BasicConnection::write(const std::string &content) { _socket->write(content); }

    time_t &BasicConnection::clock() { return _clock; }

    bool &BasicConnection::needPong() { return _needPong; }
}
