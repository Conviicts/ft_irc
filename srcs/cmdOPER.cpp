#include "IrcServer.hpp"

// Commande: OPER
// Paramètres: <utilisateur> <mot de passe>
// Le message OPER est utilisé par un utilisateur normal pour obtenir le privilège d'opérateur. La combinaison de <utilisateur> et <mot de passe> est nécessaire pour obtenir le privilège Opérateur.
// Si le client envoyant la commande OPER fournit un mot de passe correct pour l'utilisateur donné, le serveur informe le reste du réseau du nouvel opérateur en envoyant un "MODE +o" pour le pseudonyme.
// Le message OPER n'a lieu qu'entre un client et un serveur.

// Réponses numériques :
//            ERR_NEEDMOREPARAMS              RPL_YOUREOPER
//            ERR_NOOPERHOST                  ERR_PASSWDMISMATCH

int		IrcServer::OPER(User &u, Message msg) {

	if (msg.args().size() != 2) {
		u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), msg.args()[0]));
	}
	if (msg.args()[0] == "admin" && msg.args()[1] == "admin") {
		u.setOperator(true);
		u.reply(u, RPL_YOUROPER(u.nickname(), msg.args()[0], msg.args()[1]));
	}
	else
		u.reply(u, ERR_PASSWDMISMATCH(u.nickname()));
	return (1);
}