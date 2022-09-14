#pragma once

#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <stdexcept>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace TCP {
    class Socket {
        public:
            Socket();
            virtual ~Socket() throw();

            int                     fd() const;
            const sockaddr_storage  &addr() const;
            sa_family_t             family() const;
            void                    socket(int family);
            void                    bind(const sockaddr *addr, socklen_t len);
            int                     close() throw();
            void                    setReuseAddr();
            void                    setIpV6Only();
            void                    setNonBlocking(bool nonBlocking);

        protected:
            int                 _fd;
            sockaddr_storage    _addr;
        
        private:
            Socket(const Socket &other);
            Socket &operator=(const Socket &other);
    };
}
