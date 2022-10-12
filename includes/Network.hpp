#pragma once

#include <assert.h>
#include <map>
#include <list>
#include <vector>

#include "TCPServer.hpp"
#include "BasicConnection.hpp"
#include "User.hpp"
#include "Channel.hpp"

class Network {
    public:
        typedef std::map<TCP::TCPSocket *, TCP::BasicConnection *>	Connection;
        typedef std::map<std::string, User *>	                    Users;
	    typedef std::list<TCP::BasicConnection *>				    Zombies;
	    typedef std::map<std::string, Channel *>		            Channels;



        Network();
	    ~Network() throw();

        void				    add(User *user);
	    void				    add(Channel *channel);
	    void				    remove(User *user) throw();
        void                    remove(const Channel *channel) throw();
        void				    clear() throw();

	    TCP::BasicConnection    *getUserBySocket(TCP::TCPSocket *socket);
        User                    *getByNickname(const std::string &name);

        const Connection  	    &connections() const;
	    const Users  		    &users() const;
	    const Channels		    &channels() const;

        void                    newZombie(TCP::BasicConnection *z);
        TCP::BasicConnection    *nextZombie();

        Channel                 *getChannel(const std::string &name);
        Channel                 *createChannel(const std::string &name, const std::string &password, User *admin);

    private:
        Connection      _connections;
        Users            _users;
	    Zombies 		_zombies;
        Channels        _channels;

        Network(const Network &);
        Network &operator=(const Network &);
};