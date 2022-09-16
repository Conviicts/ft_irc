#include "IrcServer.hpp"

// msge: TOPIC
// Paramètres: <canal> [<sujet>]

// Le message TOPIC est utilisé pour modifier ou voir le sujet d'un canal.
// Le sujet du canal <canal> est renvoyé s'il n'y a pas de <sujet> fourni en paramètre.
// Si le paramètre <sujet> est présent, le sujet du canal changera
// si le mode du canal le permet.

// Réponses numériques :
//            ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
//            RPL_NOTOPIC                     RPL_TOPIC
//            ERR_CHANOPRIVSNEEDED

// Exemples:
// :Wiz TOPIC #test :New topic ; L'utilisateur Wiz définit le sujet.
// TOPIC #test :another topic ; Change le sujet du canal #test en "another topic".
// TOPIC #test ; Vérifie le sujet de #test.

// :Wiz TOPIC #test :New topic     ;User Wiz setting the topic.

//    TOPIC #test :another topic      ;set the topic on #test to "another
//                                    topic".

//    TOPIC #test                     ; check the topic for #test.



void	handleTopic(User &u, Message msg) {

	Channel *channel = u.getChannel();

	// std::cout << "Size: " << msg.args().size() << std::endl;
	if (msg.args().size() > 1)
	{
		if (channel->name() == msg.args()[0])
		{
			if(u.isOperator() == true) //si le user est operator
			{
				std::string message;

				for (std::vector<std::string>::const_iterator it = msg.args().begin() + 1; it != msg.args().end(); it++)
				message.append(*it + " ");
				message = message.at(0) == ':' ? message.substr(1) : message;
				channel->setTopic(message);
				channel->broadcast(": " + u.getPrefix() + " TOPIC " + channel->name() + " :" + message);
			}
			else {
			 // ERR_CHANOPRIVSNEEDED
			}
		}
		else {
			// ERR_NOTONCHANNEL
		}
	}
	if (msg.args().size() == 1) { // on verifie sil y a un topic
		if(1) { //il y a un topic
			//RPL_TOPIC
		}
		else {
			// RPL_NOTOPIC
		}
	}
}

int		IrcServer::TOPIC(User &u, Message msg) {

	if (msg.args().size() < 1) {
		return u.reply(u, ERR_NEEDMOREPARAMS(u.nickname(), msg.args()[0]));
	}

	try {
		handleTopic(u, msg);
	}
	catch(IrcServer::ChannelNotFoundException & e) {
		u.reply(u, ERR_NOSUCHNICK(u.nickname(), msg.args()[0]));
	}

	return (1);
}