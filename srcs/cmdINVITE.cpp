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

	if (msg.args().size() < 2)
		return u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), msg.args()[0]));

	User *target = _network.getByNickname(msg.args()[0]);
	if (!target)
		return u.reply(u, ERR_NOSUCHNICK(u.nickname(), msg.args()[0]));

	Channel *channel = _network.getChannel(msg.args()[1]);
	if (!channel)
		return u.reply(u, ERR_NOSUCHCHANNEL(u.nickname(), msg.args()[1]));

	if (channel->isOnChannel(&u)) {

		if (channel->isOperator(&u)) {
	
			if (channel->isOnChannel(target) == false) {

				channel->invite(target);
				target->reply(*target, RPL_INVITING(u.getPrefix(), target->nickname(), channel->name())); // use reply ou write ?
				u.reply(u, RPL_INVITING(u.getPrefix(), target->nickname(), channel->name()));
			}
			else
				u.reply(u, ERR_USERONCHANNEL(target->nickname(), channel->name()));
		}
		else
			u.reply(u, ERR_CHANOPRIVSNEEDED(u.nickname(), channel->name()));
	}
	else
		u.reply(u, ERR_NOTONCHANNEL(u.nickname(), channel->name()));

	return (1);
}