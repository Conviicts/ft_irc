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

		void						setOperator(User *user, bool op);

		void						setInviteOnly(bool mode);
		void						invite(User *user);
		void						removeInvited(User &user);

		bool						isOnChannel(User const *user) const;
		bool						isOperator(User const *user) const;
		bool						isInvited(User const & user) const;
		bool						isInviteOnly() const;

	private:
		Channel(const Channel &);
		Channel &operator=(const Channel &);

		int					_maxUsers; // size_t
		bool				_invite_only;
		User				*_admin;
		std::string			_name;
		std::string			_password;
		std::string			_topic;
		std::vector<User*>	_users;
		std::vector<User*>	_invited;
		std::vector<User*>	_operator;
};