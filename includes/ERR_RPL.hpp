#ifndef ERR_RPL_HPP
# define ERR_RPL_HPP

# include <string>

// RPL
std::string RPL_NOTICE(std::string source, std::string target, std::string message);
std::string RPL_WELCOME(std::string nick);
std::string RPL_YOURHOST(std::string nick, std::string servername, std::string version);
std::string RPL_CREATED(std::string nick, std::string date);
std::string RPL_MYINFO();
std::string RPL_UMODEIS();
std::string RPL_LISTSTART();
std::string RPL_LIST();
std::string RPL_LISTEND();
std::string RPL_NOTOPIC();
std::string RPL_TOPIC();
std::string RPL_NAMREPLY();
std::string RPL_ENDOFNAMES();
std::string RPL_MOTDSTART();
std::string RPL_ENDOFMOTD();
std::string RPL_YOUROPER(std::string nick, std::string arg1, std::string arg2);
std::string RPL_NOTONCHANNEL();

// ERR
std::string ERR_NOSUCHNICK(std::string nick, std::string arg);
std::string ERR_NOSUCHCHANNEL(std::string nick, std::string arg);
std::string ERR_CANNOTSENDTOCHAN();
std::string	ERR_TOOMANYCHANNELS(std::string nick);
std::string ERR_NONICKNAMEGIVEN(std::string nick);
std::string ERR_NICKNAMEINUSE(std::string nick);
std::string ERR_NICKCOLLISION();
std::string ERR_USERNOTINCHANNEL();
std::string ERR_NOTONCHANNEL();
std::string ERR_USERONCHANNEL();
std::string ERR_NEEDMOREPARAMS(std::string nick, std::string arg);
std::string ERR_ALREADYREGISTRED(std::string nick);
std::string ERR_ERRONEUSNICKNAME();
std::string ERR_PASSWDMISMATCH(std::string nick);
std::string ERR_CHANNELISFULL(std::string nick);
std::string ERR_UNKNOWNMODE();
std::string ERR_INVITEONLYCHAN(std::string nick);
std::string ERR_BANNEDFROMCHAN(std::string nick);
std::string ERR_BADCHANNELKEY(std::string nick);
std::string ERR_INVITEONLYCHAN();
std::string ERR_NOPRIVILEGES();
std::string ERR_CHANOPRIVSNEEDED();
std::string ERR_UMODEUNKNOWNFLAG();
std::string ERR_USERSDONTMATCH();


#endif