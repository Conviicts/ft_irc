#include "IrcServer.hpp"

// msge: TOPIC
// Paramètres: <canal> [<sujet>]

// Le message TOPIC est utilisé pour modifier ou voir le sujet d'un canal.
// Le sujet du canal <canal> est renvoyé s'il n'y a pas de <sujet> fourni en paramètre.
// Si le paramètre <sujet> est présent, le sujet du canal changera
// si le mode du canal le permet.

// Réponses numériques :
//			ERR_NEEDMOREPARAMS		ERR_NOTONCHANNEL
//			RPL_NOTOPIC				RPL_TOPIC
//			ERR_CHANOPRIVSNEEDED

// Exemples:
// :Wiz TOPIC #test :New topic			;User Wiz setting the topic.
//    TOPIC #test :another topic		;set the topic on #test to "another topic".
//    TOPIC #test						; check the topic for #test.

int		IrcServer::TOPIC(User &u, Message msg) {

	if (msg.args().size() < 1)
		return u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), msg.command()));

	Channel *channel = _network.getChannel(msg.args()[0]);
	if (!channel)
		return u.reply(u, ERR_NOSUCHCHANNEL(u.nickname(), msg.args()[0]));

	UserMode *user = channel->getUser(&u);
	if (!user)
		return u.reply(u, ERR_NOTONCHANNEL(u.nickname(), msg.args()[0]));
	if (!user->isChanOP())
		return u.reply(u, ERR_CHANOPRIVSNEEDED(u.nickname(), msg.args()[0]));
	if (msg.args().size() > 1) { 
		std::string message;
		for (std::vector<std::string>::const_iterator it = msg.args().begin() + 1; it != msg.args().end(); it++)
			message.append(*it + " ");
		message = message.at(0) == ':' ? message.substr(1) : message;
		channel->setTopic(message);
		channel->broadcast2(":" + u.getPrefix() + " TOPIC " + msg.args()[0] + " :" + message);
	} else if (msg.args().size() == 1) {
		if (channel->getTopic().empty())
			return u.reply(u, RPL_NOTOPIC(u.nickname(), msg.args()[0]));
		else
			return u.reply(u, RPL_TOPIC(u.nickname(), msg.args()[0], channel->getTopic()));
	}
	return (1);
}