#include "IrcServer.hpp"

// Commande: PART
// Paramètres: <canal>{,< canal >}

// Le message PART provoque le retrait du client expéditeur de la liste des utilisateurs actifs pour tous les canaux listés dans la chaîne de paramètres.

// Réponses numériques:

//            ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
//            ERR_NOTONCHANNEL

// Exemples:

// PART #twilight_zone ; quitte le canal "#twilight_zone"
// PART #oz-ops,&group5 ; quitte les canaux "&group5" et "#oz-ops".

std::vector<std::string>	split(const std::string &str, char delim);

int		IrcServer::PART(User &u, Message msg) {
	if (u.state() != 2)
		return (u.reply(u, ERR_NOTREGISTERED(u.nickname())));
	if (msg.args().size() < 1)
		return u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), msg.args()[0]));
	std::vector<std::string> channels = split(msg.args()[0], ',');
	for (std::vector<std::string>::const_iterator it = channels.begin(); it != channels.end(); it++) {
		Channel *channel = _network.getChannel(*it);
		if (!channel) {
			u.reply(u, ERR_NOSUCHCHANNEL(u.nickname(), *it));
			continue ;
		}
		if (channel->getUser(&u) == NULL) {
			u.reply(u, ERR_NOTONCHANNEL(u.nickname(), *it));
			continue ;
		}
		std::cout << ":" + u.getPrefix() + " PART " + channel->name() + " :Leaving" << std::endl;
		channel->broadcast2(":" + u.getPrefix() + " PART " + channel->name() + " :Leaving");
		channel->delUser(&u);
		if (channel->clientSize() == 0)
			_network.remove(channel);
	}
	return (1);
}