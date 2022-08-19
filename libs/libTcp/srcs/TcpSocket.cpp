#include "TcpSocket.hpp"

namespace TCP {

    const size_t TcpSocket::readSize = 1024;

    TcpSocket::TcpSocket():Socket(), _isReadable(false), _isWritable(false), _newline(std::string::npos) {}

    TcpSocket::TcpSocket(int listenerFd):Socket(), _isReadable(false), _isWritable(false), _newline(std::string::npos) {
        socklen_t addrLen = sizeof(_addr);
        if ((_fd = accept(listenerFd, (sockaddr *)&_addr, &addrLen)) == -1)
            throw std::runtime_error("accept() failed");
        if (family() == AF_INET) {
            _port = ntohs(((sockaddr_in *)&_addr)->sin_port);
            _ip = inet_ntoa(((sockaddr_in *)&_addr)->sin_addr);
        } else if (family() == AF_INET6) {
            char buf[INET6_ADDRSTRLEN];
            _port = ntohs(((sockaddr_in6 *)&_addr)->sin6_port);
            if (!(inet_ntop(AF_INET6, &_addr, buf, INET6_ADDRSTRLEN))) {
                close();
                throw std::runtime_error("inet_ntop() failed");
            }
            _ip = buf;
        } else {
            close();
            throw std::runtime_error("unknown address family");
        }
        char buf[NI_MAXHOST];
        if (!getnameinfo((sockaddr *)&_addr, addrLen, buf, sizeof(buf), NULL, 0, NI_NAMEREQD))
            _host = buf;
        else
            _host = _ip;
    }


    TcpSocket::~TcpSocket() throw() { close(); }

    bool TcpSocket::bufferIsEmpty() const { return (_writeBuffer.empty()); }

    size_t TcpSocket::readBufferSize() const { return (_readBuffer.size()); }

    void TcpSocket::write(const std::string &data) throw() {
        _writeBuffer += data + '\n';
    }

    bool TcpSocket::haveLine() {
        if (_newline == std::string::npos)
            if ((_newline = _readBuffer.find_first_of('\n')) == std::string::npos)
                if ((_newline = _readBuffer.find_first_of('\r')) == std::string::npos)
                    return false;
        return true;
    }

    bool TcpSocket::canRead() {
        return (haveLine() || _isReadable);
    }

    bool TcpSocket::read(std::string &line) {
        line.clear();
        if (!haveLine() && !fill())
            return false;
        if (!haveLine())
            return true;
        line = _readBuffer.substr(0, _newline + 1);
        _readBuffer.erase(0, _newline + 1);
        _newline = std::string::npos;
        return (true);
    }

    bool TcpSocket::fill() {
        if (_isReadable) {
            char buf[readSize + 1];
            int nb;
            _isReadable = false;
            if (!(nb = recv(buf, readSize)))
                return false;
            buf[nb > 0 ? nb : 0] = '\0';
            _readBuffer += buf;
        }
        return true;
    }

    void TcpSocket::flush() {
        if (!_isWritable || bufferIsEmpty())
            return ;
        _isWritable = false;
        send(_writeBuffer.c_str(), _writeBuffer.size());
        _writeBuffer.clear();
    }

    int TcpSocket::send(const void *buf, size_t n, int flags) {
        int nb;
        if ((nb = ::send(_fd, buf, n, flags)) == -1)
            if (errno != EWOULDBLOCK)
                throw std::runtime_error("send() failed");
        return (nb);
    }

    int TcpSocket::recv(void *buf, size_t n, int flags) {
        int nb;
        if ((nb = ::recv(_fd, buf, n, flags)) == -1)
            if (errno != EWOULDBLOCK)
                throw std::runtime_error("recv() failed");
        return (nb);
    }

    const std::string &TcpSocket::ip() const { return (_ip); }

    uint16_t TcpSocket::port() const { return (_port); }

    const std::string &TcpSocket::host() const { return _host; }

}
