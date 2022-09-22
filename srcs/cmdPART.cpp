#include "IrcServer.hpp"

// Commande: PART
// Paramètres: <canal>{,< canal >}

// Le message PART provoque le retrait du client expéditeur de la liste des utilisateurs actifs pour tous les canaux listés dans la chaîne de paramètres.

// Réponses numériques:

//            ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
//            ERR_NOTONCHANNEL

// Exemples:

// PART #twilight_zone ; quitte le canal "#twilight_zone"
// PART #oz-ops,&group5 ; quitte les canaux "&group5" et "#oz-ops".

std::string		takeChann(std::string arg) {

	// std::string chann = arg.pop_back();
	return arg;
}

// message.append(*it + " ");

int		IrcServer::PART(User &u, Message msg) {

	bool isPart = false;

	if (msg.args().size() < 1)
		return u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), msg.args()[0]));

	Channel *channel;
	if (msg.args().size() > 0) {

		std::string chann;
		std::string quitChann;

		for (std::vector<std::string>::const_iterator it = msg.args().begin(); it != msg.args().end(); it++) {

			chann = takeChann(*it);
			channel = _network.getChannel(chann);
			if (!channel)
				return u.reply(u, ERR_NOSUCHCHANNEL(u.nickname(), chann));

			if (channel->isOnChannel(&u)) {

				isPart = true;
				channel->delUser(&u);
				// u.removeChannelBaby(channel);
				quitChann.append(" " + channel->name());
			}
			else
				u.reply(u, ERR_NOTONCHANNEL(u.nickname(), channel->name()));
			if (isPart)
				u.write(u.getPrefix() + ": Leaving:" + channel->name());
		}
	}



	return (1);
}