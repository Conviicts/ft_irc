#pragma once
#include <vector>

#include "BasicConnection.hpp"
#include "Channel.hpp"
#include "Replies.hpp"

class Channel;

class User : public TCP::BasicConnection {
	public:

		enum Status {		// State c'est pour toi john si tu veux utiliser ca si non tu peux delete 
			UNREGISTER,
			UNREGISTER_PASS, // ?
			REGISTER,
			DISCONNECT,
		};

		User(TCP::TCPSocket *socket);
		virtual ~User();

		const std::string		&nickname() const;
		void					setNickname(const std::string &nickname);

		const std::string		&username() const;
		void					setUsername(const std::string &username);

		const std::string		&realname() const;
		void					setRealname(const std::string &realname);

		int						channelsCount();
		void					setChannelsCount(const int count);

		int						state() const;//XX on garde ?
		void					setState(int state);//XX on garde ?

		bool					isOperator() const; //XX on garde ?
		void					setOperator(bool value);//XX on garde ?

		std::string				getPrefix() const;

		std::string				getResponse(User &u, std::string const &reponse);
		int						reply(User &u, std::string const &reponse);
		void					welcome(User &u);

		void					joinChannel(User &u, Channel *channel);

	private:
		std::string				_nickname;
		std::string				_username;
		std::string				_realname;
		bool					_operator; //XX operator or not
		int						_state;
		int						_channelsCount;

	protected:
		time_t					_idle;
};