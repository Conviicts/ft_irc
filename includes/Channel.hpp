#pragma once
#include <iostream>
#include <map>
#include <set>

#include "User.hpp"
#include "UserMode.hpp"

class User;

class Channel {
	public:
		Channel(const std::string &name, const std::string &password, User *admin);
		~Channel();

		const std::string			&name() const;

		int							type() const;
		void						setType(int type);

		const std::string			&password() const;
		void						setPassword(const std::string &password);

		void						addUser(User *user, const UserMode &mode);
		void						delUser(User *user);
		UserMode 					*getUser(User *user);

		int							maxUsers() const;
		void						setMaxUsers(int maxUsers);

		const std::string			&getTopic() const;
		void						setTopic(std::string const & topic);

		void						broadcast(User *user, const std::string &msg);
		void						kick(User *user, User *target, const std::string &reason);

		int							clientSize() const;
		std::vector<std::string>	usersNick();

		bool 						canWrite(User *user);


		void						setInviteOnly(bool mode);
		void						invite(User *user);

		bool						isOnChannel(User const *user) const;
		bool						isOperator(User const *user) const;
		bool						isInvited(User const & user) const;
		bool						isInviteOnly() const;

	private:
		Channel(const Channel &);
		Channel &operator=(const Channel &);

		int							_maxUsers; // size_t
		int							_type;
		bool						_invite_only;
		User						*_admin;
		std::string					_name;
		std::string					_password;
		std::string					_topic;
		std::map<User *, UserMode>	_users;
		std::set<std::string>		_invited;
};