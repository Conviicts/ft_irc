#pragma once
#include <string>
#include <cerrno>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdexcept>

#include "Socket.hpp"

namespace TCP {
    class TcpSocket : public Socket {
        public:
            virtual ~TcpSocket() throw();
            const std::string	&ip() const;
            uint16_t			port() const;
            const std::string	&host() const;
            size_t				readBufferSize() const;

            bool				haveLine();
            bool				canRead();
            bool				read(std::string &line);
            void				write(const std::string &data) throw();

            virtual bool		fill();
            virtual void		flush();

        protected:

            TcpSocket();
            TcpSocket(int fd);

            static const size_t readSize;

            std::string	        _ip;
            uint16_t	        _port;
            std::string	        _host;
            bool		        _isReadable;
            bool		        _isWritable;
            std::string	        _readBuffer;
            std::string	        _writeBuffer;
            size_t		        _newline;

            virtual int		    recv(void *buf, size_t size, int flags = 0);
            virtual int		    send(const void *buf, size_t size, int flags = 0);
            virtual bool	    bufferIsEmpty() const;

        private:
            TcpSocket(const TcpSocket &other);
            TcpSocket &operator=(const TcpSocket &other);

            friend class TcpServer;
            friend class TcpClient;
            friend class Listener;
    };
}
