#include "IrcServer.hpp"

// Commande: MODE

// La commande MODE est une commande à utilisation duale sur IRC. Elle permet aussi bien de changer les modes des utilisateurs que ceux des canaux. La raison à ce choix est qu'un jour les pseudonymes deviendront obsolètes et la propriété équivalente sera le canal.

// Lors du traitement des messages MODE, il est recommandé de commencer par décomposer le message en entier, puis de réaliser les modifications résultantes.
// 4.2.3.1 Les modes des canaux
// Paramètres: <canal> {[+|-]|o|p|s|i|t|n|b|v} [<limite>] [<utilisateur>] [<masque de bannissement >]

// La commande MODE permet aux opérateurs de canal de changer les caractéristiques de 'leur' canal. Les serveurs doivent aussi pouvoir changer les modes du canal, de façon à pouvoir créer des opérateurs.

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

// Lors de l'utilisation des options 'o' et 'b', le nombre de paramètres est restreint à trois par commande, et ce pour n'importe quelle combinaison de 'o' et de 'b'.

// 4.2.3.2 Modes des utilisateurs
// Paramètres: <pseudonyme> {[+|-]|i|w|s|o}

// Les modes utilisateurs sont typiquement des modifications qui affectent la façon dont le client est vu par les autres, ou quels types de messages sont reçus. Une commande MODE n'est acceptée que si l'expéditeur du message et le pseudonyme donné en paramètre sont les mêmes.

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
//            ERR_UNKNOWNMODE                 ERR_NOSUCHCHANNEL

//            ERR_USERSDONTMATCH              RPL_UMODEIS
//            ERR_UMODEUNKNOWNFLAG

void	IrcServer::_userMODE(User *u, Message msg, User *target) {
	(void)u;
	(void)msg;
	(void)target;
}

void	IrcServer::_channelMODE(User *u, Message msg, Channel *channel) {

	std::string			mode;
	int					type = 0;
	char				c;
	int					limits = -1;

	if (msg.args().size() < 3)
	{
		// 324     RPL_CHANNELMODEIS
		return ;
	}
	// msg.args()[3] == user?
	User *target = _network.getByNickname(msg.args()[3]);
	if (!target) {

		// msg.args()[3] == limits?
		// est-ce que cest un chiffre ?
	}
	
	if (channel->isOperator(u)) {

		for (int i = 0; (c = msg.args()[1][i]); i++)
		{
			switch (c) {
				case '+':
				case '-':
					type = (c == '+');
					mode =+c;
					break ;
				case 'o':
					channel->setOperator(target, type);
					mode+= c;
					break;
				case 'l':
					channel->setMaxUsers(limits);
					mode+= c;
					break;
				case 'i':
					channel->setInviteOnly(type);
					mode+= c;
					continue ;
				default:
					//ERR_UMODEUNKNOWNFLAG
					break;
			}
		}
		//message target->write
		if (u != target) {

			//u->write
		}
	}
	else {
		//u->reply(ERR_CHANOPRIVSNEEDED(u->nickname(), channel->name()));
	}

}

int		IrcServer::MODE(User &u, Message msg) {

	if (msg.args().size() < 1)
		return u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), msg.args()[0]));

	/* channel MODE */
	if (msg.args()[0][0] == '#') {

		Channel *channel = _network.getChannel(msg.args()[0]);
		if (!channel)
			return u.reply(u, ERR_NOSUCHCHANNEL(u.nickname(), msg.args()[0]));
		_channelMODE(&u, msg, channel);
	}

	/* user MODE */
	else {

		User *target = _network.getByNickname(msg.args()[0]);
		if (!target)
			return u.reply(u, ERR_NOSUCHNICK(u.nickname(), msg.args()[0]));
		_userMODE(&u, msg, target);
	}

	return (1);
}