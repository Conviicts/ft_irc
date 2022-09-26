#include "IrcServer.hpp"

// Commande: LIST
// Paramètres: [<canal>{,<canal>} [<serveur>]]
// Le message LIST est utilisé pour lister les canaux et leur sujet. Si le paramètre <canal> est utilisé, seul le statut de ces canaux est affiché. Les canaux privés sont listés (sans leur sujet) comme canal "Prv" à moins que le client qui génère la requête soit effectivement sur le canal. De même, les canaux secrets ne sont pas listés du tout, à moins que le client soit un membre du canal en question.

// Réponses numériques :
//            ERR_NOSUCHSERVER                RPL_LISTSTART
//            RPL_LIST                        RPL_LISTEND

// Exemples:
// LIST ; Liste tous les canaux.
// LIST #twilight_zone,#42 ; Liste les canaux #twilight_zone et #42

// std::string		takeChann(std::string arg) {

// 	// std::string chann = arg.pop_back();
// 	return arg;
// }

int		IrcServer::LIST(User &u, Message msg) {

	if (msg.args().size() < 1) {

		u.reply(u, RPL_LISTSTART(u.nickname()));
		// for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		// 	u.reply(u, RPL_LIST(u.nickname(), (*it)->name(), (*it)->getTopic()));
		// }
		u.reply(u, RPL_LISTEND(u.nickname()));
	}

	if (msg.args().size() > 0) {

		// Channel *channel;
		// std::string chann;

		u.reply(u, RPL_LISTSTART(u.nickname()));
		for (std::vector<std::string>::const_iterator it = msg.args().begin(); it != msg.args().end(); it++) {

			// chann = takeChann(*it);
			// channel = _network.getChannel(chann);
			// if (channel)
			// 	u.reply(u, RPL_LIST(u.nickname(), channel->name(), channel->getTopic()));
		}
		u.reply(u, RPL_LISTEND(u.nickname()));
	}
	return (1);
}