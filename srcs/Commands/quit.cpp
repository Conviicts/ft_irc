#include "IrcServer.hpp"

int		IrcServer::QUIT(User &u, Message msg) {

	if (u.state() != 2)
		disconnect(u, "Client Quit", true);
	else
		disconnect(u, msg.args()[0], false);

	return (1);
}