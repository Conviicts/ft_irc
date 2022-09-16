#include "IrcServer.hpp"

// Commande: INVITE
// Paramètres: <pseudonyme> <canal>

// Le message INVITE est utilisé pour inviter des utilisateurs dans un canal. Le paramètre <pseudonyme> est le pseudonyme de la personne à inviter dans le canal destination <canal>. Il n'est pas nécessaire que le canal dans lequel la personne est invitée existe, ni même soit valide. Pour inviter une personne dans un canal en mode sur invitation (MODE +i), le client envoyant l'invitation doit être opérateur sur le canal désigné.

// Réponses numériques :
//            ERR_NEEDMOREPARAMS              ERR_NOSUCHNICK
//            ERR_NOTONCHANNEL                ERR_USERONCHANNEL
//            ERR_CHANOPRIVSNEEDED
//            RPL_INVITING                    RPL_AWAY

// Exemples:
// :Angel INVITE Wiz #Dust ; L'utilisateur Angel invite WiZ sur le canal #Dust
// INVITE Wiz #Twilight_Zone ; Commande pour inviter WiZ sur #Twilight_zone

int		IrcServer::INVITE(User &u, Message msg) {
	(void)u;
	(void)msg;

	std::cout << "TODO: INVITE" << std::endl;


	return (1);
}