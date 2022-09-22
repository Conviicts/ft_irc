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

void	handleTopic(User &u, Message msg, Network &network) {

	Channel *channel = network.getChannel(msg.args()[0]);

	// std::cout << "Size: " << msg.args().size() << std::endl;
	if (msg.args().size() > 1) {
		if (channel->isOnChannel(&u)) { //si le user est dans le channel
			if(channel->isOperator(&u)) { //si le user est operator

				/* CHECK fonction message --------------------------------------------------------------------------- */
				std::string message;

				for (std::vector<std::string>::const_iterator it = msg.args().begin() + 1; it != msg.args().end(); it++)
				message.append(*it + " ");
				message = message.at(0) == ':' ? message.substr(1) : message;
				/* -------------------------------------------------------------------------------------------------- */
				channel->setTopic(message);
				channel->broadcast(": " + u.getPrefix() + " TOPIC " + channel->name() + " :" + message);
			}
			else
				u.reply(u, ERR_CHANOPRIVSNEEDED(u.nickname(), channel->name()));
		}
		else
			u.reply(u, ERR_NOTONCHANNEL(u.nickname(), channel->name()));
	}

	if (msg.args().size() == 1) { // on verifie sil y a un topic
		if(channel->getTopic().size()) //il y a un topic
			u.reply(u, RPL_TOPIC(u.nickname(), channel->name(), channel->getTopic()));
		else
			u.reply(u, RPL_NOTOPIC(u.nickname(), channel->name()));
	}
}

int		IrcServer::TOPIC(User &u, Message msg) {

	if (msg.args().size() < 1)
		return u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), msg.args()[0]));

	try {
		handleTopic(u, msg, _network);
	}
	catch(IrcServer::ChannelNotFoundException & e) {
		u.reply(u, ERR_NOSUCHCHANNEL(u.nickname(), msg.args()[0]));
	}
	catch(IrcServer::UserNotFoundException & e) {
		u.reply(u, ERR_NOSUCHNICK(u.nickname(), msg.args()[1]));
	}

	return (1);
}