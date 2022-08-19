#pragma once
#include "TcpSocket.hpp"

class BasicConnection {
    public:
        enum Type {
            USER,
            SERVICE
        };
        virtual ~BasicConnection();

        Type                type() const;
        void                setType(const Type type);

        TCP::TcpSocket      *socket() const;
        unsigned            count() const;

        void                write(const std::string &str);

        bool                &needPong();
    
    protected:
        BasicConnection(TCP::TcpSocket *socket, Type type, unsigned count = 0);

        TCP::TcpSocket      *_socket;
        Type                _type;
        unsigned            _count;
        bool                _needPong;
};