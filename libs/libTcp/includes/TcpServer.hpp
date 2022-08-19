#pragma once
#include <list>
#include <netdb.h>
#include <sys/select.h>
#include <sys/signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdexcept>
#include <memory>

#include "Listener.hpp"
#include "TcpSocket.hpp"

namespace TCP {
    class TcpServer {
        public:
            TcpServer();
            virtual ~TcpServer() throw();

            size_t				connectionsSize() const;
            void				setMaxConnections(size_t MaxConnections);


            void				listen(const std::string &port, size_t maxQueueLen = 5);
            void				disconnect(TcpSocket *client) throw();

            TcpSocket			*getNewConnection() throw();
            TcpSocket			*getPendingConnection() throw();

            void				select();

            class SigintException : public std::exception {};

        private:

            static const size_t     defaultMaxConnections;
            static const timeval    selectTimeout;

            addrinfo				_hint;
            std::list<Listener*>	_listeners;
            std::list<Listener*>	_incoming;
            std::list<TcpSocket*>	_connections;
            std::list<TcpSocket*>	_pending;
            size_t					_maxConnections;

            TcpServer(const TcpServer &);
            TcpServer &operator=(const TcpServer &);
    };
}
