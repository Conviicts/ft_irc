#include "IrcServer.hpp"
#include "Replies.hpp"

// Commande: NOTICE
// Paramètres: <pseudonyme> <texte>

// Le message NOTICE s'utilise de la même façon que PRIVMSG.
// La différence entre NOTICE et PRIVMSG est qu'aucune réponse automatique
// ne doit être envoyée en réponse à un message NOTICE.
// Ceci est aussi valable pour les serveurs - ils ne doivent pas renvoyer
// de message d'erreur à la réception d'un message NOTICE.
// Le but de cette règle est d'éviter les boucles entre les clients
// qui enverraient automatiquement quelque chose en réponse à une requête.
// Cela est typiquement utilisé par des automates (des clients qui ont une
// intelligence artificielle ou un autre programme interactif contrôlant leurs actions)
// qui répondent systématiquement aux réponses d'autres automates.

// Voir PRIVMSG pour les détails sur les réponses, et pour les exemples.

int		IrcServer::NOTICE(User &u, Message msg) {
	(void)u;
	(void)msg;

	if (msg.args().size() < 2)
		return (0);
	std::string target = msg.args()[0];

	/* CHECK fonction message --------------------------------------------------------------------------- */
	std::string message;

	for (std::vector<std::string>::const_iterator it = msg.args().begin() + 1; it != msg.args().end(); ++it)
		message.append(*it + " ");
	message = message.at(0) == ':' ? message.substr(1) : message;
	/* -------------------------------------------------------------------------------------------------- */

	if (target.at(0) == '#') {
		Channel *c = _network.getChannel(target);
		if (c)
			c->broadcast(&u, "NOTICE " + c->name() + " :" + message);
	} else {
		User *t = _network.getByNickname(target);
		if (t)
			t->write("NOTICE " + target + " :" + message);
	}
	return (1);
}