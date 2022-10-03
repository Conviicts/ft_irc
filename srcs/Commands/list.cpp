#include "IrcServer.hpp"

// Commande: LIST
// Paramètres: [<canal>{,<canal>} [<serveur>]]
// Le message LIST est utilisé pour lister les canaux et leur sujet.
// Si le paramètre <canal> est utilisé, seul le statut de ces canaux est affiché.
// Les canaux privés sont listés (sans leur sujet) comme canal "Prv" à moins
// que le client qui génère la requête soit effectivement sur le canal.
// De même, les canaux secrets ne sont pas listés du tout,
// à moins que le client soit un membre du canal en question.

// Réponses numériques :
//            ERR_NOSUCHSERVER                RPL_LISTSTART
//            RPL_LIST                        RPL_LISTEND

// Exemples:
// LIST ; Liste tous les canaux.
// LIST #twilight_zone,#42 ; Liste les canaux #twilight_zone et #42

std::vector<std::string>    split(const std::string &str, char delim);

int		IrcServer::LIST(User &u, Message msg) {

	if (u.state() != 2)
		return (u.reply(u, ERR_NOTREGISTERED(u.nickname())));
	if (!msg.args().size()) { // lister tous les chanels

		u.reply(u, RPL_LISTSTART(u.nickname()));
		for (Network::Channels::const_iterator it = _network.channels().begin(); it != _network.channels().end(); ++it) {
			u.reply(u, RPL_LIST(u.nickname(), (*it).second->name(), (*it).second->getTopic()));
		}
		u.reply(u, RPL_LISTEND(u.nickname()));
	} else if (msg.args().size() > 0) { // afficher le status du ou des chanels
		std::vector<std::string> targets = split(msg.args()[0], ',');
		for (std::vector<std::string>::const_iterator it = targets.begin(); it != targets.end(); it++) {
			Channel *channel = _network.getChannel(*it);
			if (!channel)
				return u.reply(u, ERR_NOSUCHCHANNEL(u.nickname(), *it));
			u.reply(u, RPL_LIST(u.nickname(), channel->name(), channel->getTopic()));
		}
	}
	return (1);
}