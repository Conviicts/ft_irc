#pragma once
#include <iostream>

#include "User.hpp"

class User;

class Channel {
	public:
		Channel(const std::string &name, const std::string &password, User *admin);
		~Channel();

		User						*admin() const;
		const std::string			&name() const;

		const std::string			&password() const;
		void						setPassword(const std::string &password);

		void						addUser(User *user);
		void						delUser(User *user);

		int							 maxUsers() const;
		void						setMaxUsers(int maxUsers);

		const std::string			&getTopic() const;
		void						setTopic(std::string const & topic);

		void						broadcast(const std::string &msg);
		void						kick(User *user, User *target, const std::string &reason);

		int							clientSize() const;
		std::vector<std::string>	usersNick();

		bool						isOnChannel(User const & nick) const;

	private:
		Channel(const Channel &);
		Channel &operator=(const Channel &);

		std::string			_name;
		std::string			_password;
		std::string			_topic;
		User				*_admin;
		int					_maxUsers;
		std::vector<User *>	_users;
        std::vector<User*>	_invited;
        std::vector<User*>	_operator;
};