#pragma once

#include "BasicConnection.hpp"

class User : public TCP::BasicConnection {
    public:
        User(TCP::TCPSocket *socket);
        virtual ~User();

        const std::string       &nickname() const;
        void					setNickname(const std::string &nickname);

    private:
        std::string             _nickname;

    protected:
        time_t          _idle;
};