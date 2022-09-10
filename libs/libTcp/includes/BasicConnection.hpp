#pragma once
#include "TCPSocket.hpp"

namespace TCP {
    class BasicConnection {
        public:
            virtual ~BasicConnection();

            TCP::TCPSocket      *socket() const;
            unsigned            count() const;

            void                write(const std::string &str);

            bool                &needPong();
            time_t              &clock();
        
        protected:
            BasicConnection(TCP::TCPSocket *socket, unsigned count = 0);

            TCPSocket           *_socket;
            unsigned            _count;
            bool                _needPong;
            time_t              _clock;
    };
}