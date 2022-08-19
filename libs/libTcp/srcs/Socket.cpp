#include "Socket.hpp"

namespace TCP {

    Socket::Socket() : _fd(-1) {}

    Socket::~Socket() throw() {}

    int Socket::fd() const { return (_fd); }

    const sockaddr_storage &Socket::addr() const { return (_addr); }

    sa_family_t Socket::family() const { return (_addr.ss_family); }

    void Socket::socket(int family)
    {
        if ((_fd = ::socket(family, SOCK_STREAM, IPPROTO_TCP)) == -1)
            throw std::runtime_error("socket() failed");
    }

    int Socket::close() throw()
    {
        return ::close(_fd);
    }

    void Socket::bind(sockaddr *addr, socklen_t addr_size)
    {
        if (::bind(_fd, addr, addr_size) == -1)
            throw std::runtime_error("bind() failed");
        std::memcpy(&_addr, &addr, addr_size);
    }

    void Socket::setReuseAddr()
    {
        int reuse = 1;
        if (::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1)
            throw std::runtime_error("setsockopt() failed");
    }

    void Socket::setIpv6Only()
    {
        int v6Only = 1;
        if (::setsockopt(_fd, IPPROTO_IPV6, IPV6_V6ONLY, &v6Only, sizeof(v6Only)) == -1)
            throw std::runtime_error("setsockopt() failed");
    }

    void Socket::setNonBlocking(bool on)
    {
        if (on) {
            if (fcntl(_fd, F_SETFL, O_NONBLOCK) == -1)
                throw std::runtime_error("fcntl() failed");
        } else {
            int oldfl;
            if ((oldfl = fcntl(_fd, F_GETFL)) == -1)
                throw std::runtime_error("fcntl() failed");
            fcntl(_fd, F_SETFL, oldfl & ~O_NONBLOCK);
        }
    }

}
