#pragma once
#include <iostream>

#include "User.hpp"

class User;

class Channel {
    public:
        Channel(const std::string &name, const std::string &password, User *admin);
        ~Channel();

        User                        *admin() const;
        const std::string           &name() const;

        const std::string           &password() const;
        void                        setPassword(const std::string &password);

        void                        addUser(User *user);
        void                        delUser(User *user);

        int                         maxUsers() const;
        void                        setMaxUsers(int maxUsers);

        void                        broadcast(const std::string &msg);
        void                        kick(User *user, User *target, const std::string &reason);

    	int						    clientSize() const;
    	std::vector<std::string>	usersNick();

    private:
        Channel(const Channel &);
        Channel &operator=(const Channel &);

        std::string         _name;
        std::string         _password;
        User                *_admin;
        std::vector<User *> _users;
        int                 _maxUsers;
};