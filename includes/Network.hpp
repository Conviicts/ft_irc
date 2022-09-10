#pragma once

#include <assert.h>
#include <map>
#include <list>
#include <vector>

#include "TCPServer.hpp"
#include "BasicConnection.hpp"
#include "User.hpp"

class Network {
    public:
        typedef std::map<TCP::TCPSocket *, TCP::BasicConnection *>	Connection;
        typedef std::map<std::string, User *>	                    Users;
	    typedef std::list<TCP::BasicConnection *>				    Zombies;



        Network();
	    ~Network() throw();

        void				    add(User *user);
	    void				    remove(User *user) throw();
        void				    clear() throw();
	    TCP::BasicConnection    *getUserBySocket(TCP::TCPSocket *socket);

        const Connection  	    &connections() const;
	    const Users  		    &users() const;

        void                    newZombie(TCP::BasicConnection *z);
        TCP::BasicConnection    *nextZombie();



    private:
        Connection      _connections;
        Users            _users;
	    Zombies 		_zombies;


        Network(const Network &);
        Network &operator=(const Network &);
};