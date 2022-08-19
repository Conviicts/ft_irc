#include "TcpClient.hpp"

namespace TCP {
    TcpClient::TcpClient(const std::string &host, const std::string &port, bool ipv6) {
        signal(SIGPIPE, SIG_IGN);
        _socket = new TcpSocket();
        try {
            connect(host, port, ipv6);
        } catch (std::exception &e) { 
            delete _socket; throw;
        }
    }

    TcpClient::~TcpClient() { delete _socket; }

    void TcpClient::connect(const std::string &host, const std::string &port, bool ipv6) {
        addrinfo hint, *head, *node;
        bzero(&hint, sizeof(hint));
        hint.ai_family = ipv6 ? AF_INET6 : AF_INET;
        hint.ai_socktype = SOCK_STREAM;
        hint.ai_protocol = IPPROTO_TCP;

        if (getaddrinfo(host.c_str(), port.c_str(), &hint, &head))
            throw std::runtime_error("getaddrinfo");
        try {
            for (node = head; node != NULL; node = node->ai_next) {
                _socket->socket(node->ai_family);
                if (_socket->fd() == -1)
                    continue ;
                if (::connect(_socket->fd(), node->ai_addr, node->ai_addrlen) != -1)
                    break;
                _socket->close();
            }
        } catch (std::exception &e) {
            freeaddrinfo(head);
            throw ;
        }
        freeaddrinfo(head);
    }

    void TcpClient::write(const std::string &line) {
        _socket->write(line);
    }

    void TcpClient::flush() {
        _socket->_isWritable = true;
        _socket->flush();
    }

    bool TcpClient::read(std::string &line) {
        _socket->_isReadable = true;
        _socket->setNonBlocking(true);
        bool status = _socket->read(line);
        _socket->setNonBlocking(false);
        return status;
    }

    bool TcpClient::wait(std::string &line) {
        _socket->_isReadable = true;
        return _socket->read(line);
    }
}
