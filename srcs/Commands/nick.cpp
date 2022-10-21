#include "IrcServer.hpp"

int	checkNickname(const std::string &nickname)
{
	const char chars[11] = "-[]\\`^{}_ ";

	if (nickname.size() < 3 || nickname.size() > 20)
		return (0);
	for (unsigned int i = 0; i < nickname.size(); i++) {
		if (!std::isalnum(nickname[i]))
			return 0;
	}
	if (nickname.find_first_of(chars) == std::string::npos)
		return 1;
	return 0;
}

int		IrcServer::NICK(User &u, Message msg) {

	if (!msg.args().size() || msg.args()[0].empty())
		return u.reply(u, ERR_NONICKNAMEGIVEN(u.nickname()));
	if (!checkNickname(msg.args()[0]))
		return u.reply(u, ERR_ERRONEUSNICKNAME(u.nickname(), msg.args()[0]));
	if (_network.getByNickname(msg.args()[0])) {
		u.reply(u, ERR_NICKNAMEINUSE(u.nickname()));
		return 0;
	}
	_network.remove(&u);
	u.setNickname(msg.args()[0]);
	_network.add(&u);
	return (1);
}
