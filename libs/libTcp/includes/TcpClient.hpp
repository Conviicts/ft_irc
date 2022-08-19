#pragma once
#include <netdb.h>
#include <signal.h>
#include <stdexcept>

#include "TcpSocket.hpp"

namespace TCP {
    class TcpClient {
        public:
            TcpClient(const std::string &host, const std::string &port, bool ipv6);
            virtual ~TcpClient();

            bool wait(std::string &line);

            bool read(std::string &line);
            void write(const std::string &line);
            void flush();

        private:
            TcpClient(const TcpClient &);
            TcpClient &operator=(const TcpClient &);
            
            TcpSocket *_socket;

            const TcpSocket &socket() const;
            void            connect(const std::string &host, const std::string &port, bool ipv6);

    };

}
