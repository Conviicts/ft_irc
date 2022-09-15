#pragma once
#include <vector>

#include "BasicConnection.hpp"
#include "Channel.hpp"
#include "ERR_RPL.hpp"

class Channel;

class User : public TCP::BasicConnection {
    public:

        enum Status {            // State // c'est pour toi john si tu veux utiliser ca si non tu peux delete 
			UNREGISTER,
			UNREGISTER_PASS,
			REGISTER,
			DISCONNECT,
		};

        User(TCP::TCPSocket *socket);
        virtual ~User();

        const std::string       &nickname() const;
        void					setNickname(const std::string &nickname);

        const std::string       &username() const;
        void					setUsername(const std::string &username);

        const std::string       &realname() const;
        void					setRealname(const std::string &realname);

        int                     state() const;
        void					setState(int state);

        bool                    isRegistered() const;

        bool                    isOperator() const;
        void                    setOperator(bool value);

        std::string             getPrefix() const;

        std::string             getResponse(User &u, std::string const &reponse);
        int                     reply(User &u, std::string const &reponse);
        void                    welcome(User &u);

        Channel                 *getChannel() const;
        void                    joinChannel(User &u, Channel *channel);
	    void                    setChannel(Channel *channel);

    private:
        Channel                 *_channel;
        std::string             _nickname;
        std::string             _username;
        std::string             _realname;
        bool                    _registered;
        bool                    _mode; // operator or not
        int                     _state;

    protected:
        time_t                  _idle;
};