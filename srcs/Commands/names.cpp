#include "IrcServer.hpp"

// Commande: NAMES
// Paramètres: [<canal>{,<canal>}]

// En utilisant la commande NAMES, un utilisateur peut obtenir 
// la liste des pseudonymes visibles sur n'importe quel canal qu'il peut voir.
// Les noms de canaux qu'il peut voir sont ceux qui ne sont ni privés (+p),
// ni secrets (+s), ou ceux sur lesquels il est actuellement.
// Le paramètre <canal> spécifie quels sont les canaux dont l'information est voulue,
// s'ils sont valides. Il n'y a pas de message d'erreur pour les noms de canaux invalides.
// Si le paramètre <canal> n'est pas donné, la liste de tous les canaux
// et de leurs occupants est renvoyée. A la fin de cette liste, sont listés
// les utilisateurs visibles, mais qui n'appartiennent à aucun canal visible.
// Ils sont listés comme appartenant au 'canal' "*".

// Réponses numériques:

//            RPL_NAMREPLY                    RPL_ENDOFNAMES

//  353     RPL_NAMREPLY
//                         "<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
//         366     RPL_ENDOFNAMES
//                         "<channel> :End of /NAMES list"
// Exemples:

// NAMES #twilight_zone,#42 ; liste les utilisateurs visibles sur #twilight_zone et #42, si ces canaux vous sont visibles.
// NAMES ; liste tous les canaux, et tous les utilisateurs visibles.


std::vector<std::string>	split(const std::string &str, char delim);

int		IrcServer::NAMES(User &u, Message msg) {
	std::cout << "NAMES" << std::endl;	
	if (u.state() != 2)
		return (u.reply(u, ERR_NOTREGISTERED(u.nickname())));
	if (msg.args().size() < 1)
		return u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), msg.args()[0]));
	// const Network::Channels &channels = _network.channels();
	if (msg.args().size()) {
		std::string users;
		std::vector<std::string> targets = split(msg.args()[0], ',');
		for (std::vector<std::string>::const_iterator it = targets.begin(); it != targets.end(); it++) {
			Channel *channel = _network.getChannel(*it);
			
			if (channel && channel->getUser(&u)) {
				for (std::map<User *, UserMode>::const_iterator it = channel->_users.begin(); it != channel->_users.end(); it++) {
					if ((*it).second.isChanOP())
						users += "@" + (*it).first->nickname() + " ";
					else
						users += (*it).first->nickname() + " ";
				}
				u.reply(u, RPL_NAMREPLY(u.nickname(), channel->name(), users));
			}
		}
		u.reply(u, RPL_ENDOFNAMES(u.nickname(), msg.args()[0]));
	}
	return (1);
}
