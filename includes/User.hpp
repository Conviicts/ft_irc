#pragma once

#include "BasicConnection.hpp"

class User : public TCP::BasicConnection {
    public:
        User(TCP::TCPSocket *socket);
        virtual ~User();

        const std::string       &nickname() const;
        void					setNickname(const std::string &nickname);

        int                     state() const;
        void					setState(int state);

        bool                    isRegistered() const;

    private:
        std::string             _nickname;
        bool                    _registered;
        int                     _state;

    protected:
        time_t          _idle;
};