#include <sys/socket.h>

namespace TCP {
    class Socket {
        public:
            Socket();
	        virtual ~Socket() throw();

            int                         fd() const;
            const sockaddr_storage      &addr() const;
            sa_family_t                 family() const;
            void                        socket(int family);
            int                         close() throw();
            void                        bind(sockaddr *addr, socklen_t addrlen);
            void                        setReuseAddr();
            void                        setIpv6Only();
            void                        setNonBlocking(bool on);
        
        protected:
            int                         _fd;
            sockaddr_storage            _addr;

        private:
            Socket(const Socket &other);
	        Socket &operator=(const Socket &other);
    };
}