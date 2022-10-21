#include "IrcServer.hpp"
#include "Replies.hpp"

// Commande: MODE

// La commande MODE est une commande à utilisation duale sur IRC.
// Elle permet aussi bien de changer les modes des utilisateurs que ceux des canaux.
// La raison à ce choix est qu'un jour les pseudonymes
// deviendront obsolètes et la propriété équivalente sera le canal.

// Lors du traitement des messages MODE, il est recommandé de commencer
// par décomposer le message en entier, puis de réaliser les modifications résultantes.

// 4.2.3.1 Les modes des canaux
// Paramètres: <canal> {[+|-]|o|p|s|i|t|n|b|v} [<limite>] [<utilisateur>] [<masque de bannissement >]

// La commande MODE permet aux opérateurs de canal de changer les caractéristiques de 'leur' canal.
// Les serveurs doivent aussi pouvoir changer les modes du canal, de façon à pouvoir créer des opérateurs.

// Les modes disponibles pour les canaux sont les suivants :
// o - donne/retire les privilèges d'opérateur de canal
// p - drapeau de canal privé
// s - drapeau de canal secret
// i - drapeau de canal accessible uniquement sur invitation
// t - drapeau de sujet de canal modifiable uniquement par les opérateurs
// n - pas de messages dans un canal provenant de clients à l'extérieur du canal
// m - canal modéré
// l - définit le nombre maximal de personnes dans un canal
// b - définit un masque de bannissement pour interdire l'accès à des utilisateurs
// v - donne/retire la possibilité de parler dans un canal modéré
// k - définit la clé du canal (mot de passe)

// Lors de l'utilisation des options 'o' et 'b', le nombre de paramètres est restreint à trois par commande,
// et ce pour n'importe quelle combinaison de 'o' et de 'b'.

// 4.2.3.2 Modes des utilisateurs
// Paramètres: <pseudonyme> {[+|-]|i|w|s|o}

// Les modes utilisateurs sont typiquement des modifications qui affectent
// la façon dont le client est vu par les autres, ou quels types de messages sont reçus.
// Une commande MODE n'est acceptée que si l'expéditeur du message
// et le pseudonyme donné en paramètre sont les mêmes.

// Les modes disponibles sont :
// i - marque un utilisateur comme invisible ;
// s - marque un utilisateur comme recevant les notifications du serveur ;
// w - l'utilisateur reçoit les WALLOPs ;
// o - drapeau d'opérateur.

// D'autres modes seront disponibles plus tard.

// Si un utilisateur tente de devenir opérateur en utilisant le drapeau "+o", 
// la tentative doit être ignorée. Par contre, il n'y a pas de restriction à 
// ce que quelqu'un se 'deoppe' (en utilisant "-o").

// Réponses numériques :

//            ERR_NEEDMOREPARAMS              RPL_CHANNELMODEIS
//            ERR_CHANOPRIVSNEEDED            ERR_NOSUCHNICK
//            ERR_NOTONCHANNEL                ERR_KEYSET
//            RPL_BANLIST                     RPL_ENDOFBANLIST
//            472 ERR_UNKNOWNMODE                 ERR_NOSUCHCHANNEL

//            ERR_USERSDONTMATCH              RPL_UMODEIS
//            501 ERR_UMODEUNKNOWNFLAG

#include <cstdlib>

# define isMINUS		1 << 0		// -> 1
# define isOPERATOR		1 << 1		// -> 2
# define isINVITED		1 << 2		// -> 4
# define isLIMIT		1 << 3		// -> 8



char	parseARG1(std::string arg1) {

	char	mask = 0;

	if (arg1[0] == '-')
		mask |= isMINUS;
	for (size_t i = 1; i < arg1.size(); i++) {

		if (arg1[i] == 'o')
			mask |= isOPERATOR;
		else if (arg1[i] == 'i')
			mask |= isINVITED;
		else if (arg1[i] == 'l')
			mask |= isLIMIT;
		else
			return 0;
	}
	return mask;
}

int		parseLimit(std::string str) {
	int limit = 0;

	if (str.size() > 2)
		return -1;
	for (std::string::iterator it = str.begin(); it != str.end(); it++) {
		if (!std::isdigit(*it))
			return -1;
	}
	limit = std::atoi(str.c_str());
	return limit;
}

void	IrcServer::_channelMODE(User *u, Message msg, Channel *channel) {
	char				mask = 0;

	if (msg.args().size() < 2)
		return ;
	if (msg.args().size() > 1) {
		if ((mask = parseARG1(msg.args()[1])) == 0) {
			u->reply(*u, ERR_UMODEUNKNOWNFLAG(u->nickname(), msg.args()[1]));
			return ;
		}
	}
	UserMode *t = NULL;
	User *target = NULL;
	int limit = -1;
	std::string l = "";

	if (msg.args().size() > 2) {
		if ((limit = parseLimit(msg.args()[2])) > 0) {
			l = msg.args()[2];
		} else {
			t = channel->getUserByNickname(msg.args()[2]); 
			if (!t)
				return ;
			target = _network.getByNickname(msg.args()[2]);
		}
	}

	if (msg.args().size() > 3) {
		if (limit == -1 && (limit = parseLimit(msg.args()[3])) > 0) {
			l = msg.args()[3];
		} else if (t == NULL) {
			t = channel->getUserByNickname(msg.args()[3]);
			if (!t)
				return ;
			target = _network.getByNickname(msg.args()[3]);
		} else {
			return;
		}
	}
	UserMode *user = channel->getUser(u);
	
	if (user->isChanOP()) {

		if (msg.args()[1].empty())
			return ;
		if (t != NULL && mask & isOPERATOR && user != t) {
			if (mask & isMINUS) {
				t->setChanOP(false);
				target->write(":" + u->nickname() + " MODE " + channel->name() + " -o " + target->nickname());
					if (u != target)
				u->write(":" + u->nickname() + " MODE " + channel->name() + " -o " + target->nickname());
			} else { 
				t->setChanOP(true);
				target->write(":" + u->nickname() + " MODE " + channel->name() + " +o " + target->nickname());
					if (u != target)
				u->write(":" + u->nickname() + " MODE " + channel->name() + " +o " + target->nickname());
			}
		}

		if (mask & isINVITED) {
			if (mask & isMINUS) {
				channel->setInviteOnly(false);
				channel->broadcast2(":" + u->nickname() + " MODE " + channel->name() + " -i");
			} else {
				channel->setInviteOnly(true); 
				channel->broadcast2(":" + u->nickname() + " MODE " + channel->name() + " +i");
			}
		}
		if (limit != -1 && mask & isLIMIT) {
			if (mask & isMINUS) {
				channel->setMaxUsers(99);
				channel->broadcast2(":" + u->nickname() + " MODE " + channel->name() + " -l");
			} else {
				channel->setMaxUsers(limit);
				channel->broadcast2(":" + u->nickname() + " MODE " + channel->name() + " +l " + l);
			}
		}
	}
	else { u->reply(*u, ERR_CHANOPRIVSNEEDED(u->nickname(), channel->name())); }
}

void	IrcServer::_userMODE(User *u, Message msg, User *target) {

	if (msg.args().size() == 2 && msg.args()[1] == "+i")
		return;
	if (u != target) {
		u->reply(*u, ERR_USERSDONTMATCH(u->nickname()));
		return ;
	}
	if (u->isOperator()) {
		if (msg.args()[1] == "-o")
			u->setOperator(false);
		else if (msg.args()[1] == "+o")
			return ;
		else {
			u->reply(*u, ERR_UMODEUNKNOWNFLAG(u->nickname(), msg.args()[1]));
			return ;
		}
		u->reply(*u, ":" + u->getPrefix() + " MODE " + u->nickname() + " " + msg.args()[1]);
	} else {
		u->reply(*u, ERR_NOPRIVILEGES(u->nickname()));
	}
}

int		IrcServer::MODE(User &u, Message msg) {

	if (msg.args().size() > 1) {

		if (msg.args()[0][0] == '#') {
			Channel *channel = _network.getChannel(msg.args()[0]);
			if (!channel)
				return u.reply(u, ERR_NOSUCHCHANNEL(u.nickname(), msg.args()[0]));
			_channelMODE(&u, msg, channel);
		} else {
			User *target = _network.getByNickname(msg.args()[0]);
			if (!target)
				return u.reply(u, ERR_NOSUCHNICK(u.nickname(), msg.args()[0]));
			_userMODE(&u, msg, target);
		}
	}
	return (1);
}
