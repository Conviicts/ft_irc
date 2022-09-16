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



int		IrcServer::MODE(User &u, Message msg) {
	(void)u;
	(void)msg;
	std::cout << "MODE commande" << std::endl;

	if (msg.args().size() < 1) {
		std::cout << "ERR_NEEDMOREPARAMS" << std::endl;
	}
	
	// User MODE
	// Paramètres: <pseudonyme> {[+|-]|i|w|s|o}
	if (msg.args().size() == 2) {
		//check u.nickname()
		if (u.nickname() != msg.args()[0]) {
			std::cout << "ERR_NOSUCHNICK" << std::endl;
		}
		if (msg.args()[1] == "-o") {
			// le user se 'deoppe'
		}
	}

	// Channel MODE
	// Paramètres: <canal> {[+|-]|o|p|s|i|t|n|b|v} [<limite>] [<utilisateur>] [<masque de bannissement >]
	if (msg.args().size() == 5) {
		Channel *channel;

		channel = _network.getChannel(msg.args()[0]);
		if (!channel) {
			std::cout << "ERR_NOSUCHCHANNEL" << std::endl;
		}

		// check msg[2] = flags : {[+|-]|o|p|s|i|t|n|b|v}
		// check msg[3] = <limite>
		// check msg[4] = <user>
		// check msg[5] = <masque de bannissement >
	}

	return (1);
}