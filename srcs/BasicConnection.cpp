#include "BasicConnection.hpp"

BasicConnection::BasicConnection(TCP::TcpSocket *socket, Type type, unsigned count) :
    _socket(socket),
    _type(type),
    _count(count),
    _needPong(false)
{}

BasicConnection::~BasicConnection() {}

BasicConnection::Type BasicConnection::type() const {
	return (_type);
}

TCP::TcpSocket *BasicConnection::socket() const {
	return (_socket);
}

unsigned BasicConnection::count() const {
	return (_count);
}

void BasicConnection::setType(const Type type) {
	_type = type;
}

void BasicConnection::write(const std::string &str) {
	_socket->write(str);
}

bool &BasicConnection::needPong() {
	return _needPong;
}
