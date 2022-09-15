# include "ERR_RPL.hpp"

// std::string RPL_WELCOME(std::string nick, std::string user, std::string host) {
//    return "001 " + nick +" :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host);
// }

/* -------------------------------------------------------------------------- */
/* REPLIES                                                                    */
/* -------------------------------------------------------------------------- */

std::string RPL_WELCOME(std::string nick) {
    return "001 "+ nick +" :Welcome to the Internet Relay Network!";
}

std::string RPL_YOURHOST(std::string nick, std::string servername, std::string version) {
    return "002 "+ nick + " Your host is " + servername + ", running version " + version;
}

std::string RPL_CREATED(std::string nick, std::string date) {
    return "003 "+ nick + " This server was created " + date;
}

std::string RPL_MYINFO() {
    return "004 ";
}

std::string RPL_UMODEIS() {
    return "221 ";
}

std::string RPL_LISTSTART() {
    return "321 ";
}

std::string RPL_LIST() {
    return "322 ";
}

std::string RPL_LISTEND() {
    return "323 ";
}

std::string RPL_NOTOPIC() {
    return "331 ";
}

std::string RPL_TOPIC() {
    return "332 ";
}

std::string RPL_NAMREPLY() {
    return "353 ";
}

std::string RPL_ENDOFNAMES() {
    return "366 ";
}

std::string RPL_MOTDSTART() {
    return "375 ";
}

std::string RPL_ENDOFMOTD() {
    return "376 ";
}

std::string RPL_YOUROPER() {
    return "381 ";
}

std::string RPL_NOTONCHANNEL() {
    return "442 ";
}


/* -------------------------------------------------------------------------- */
/* ERRORS                                                                     */
/* -------------------------------------------------------------------------- */

std::string ERR_CLOSINGLINK() {
    return "ERR_COSINGLINK";
}

std::string ERR_NOSUCHNICK(std::string nick, std::string arg) {
    return "401 " + nick + ": " + arg + ":No such nick";
}

std::string ERR_NOSUCHCHANNEL(std::string nick, std::string arg) {
    return "403 " + nick + ": " + arg + ":No such channel";
}

std::string ERR_CANNOTSENDTOCHAN() {
    return "404 ";
}

std::string ERR_NONICKNAMEGIVEN(std::string nick) {
    return "431 " + nick + ":No nickname given";
}

std::string ERR_NICKNAMEINUSE(std::string nick) {
    return "433 " + nick + ":Nickname is already in use";
}

std::string ERR_NICKCOLLISION() {
    return "436 ";
}

std::string ERR_USERNOTINCHANNEL() {
    return "441 ";
}

std::string ERR_NOTONCHANNEL() {
    return "442 ";
}

std::string ERR_USERONCHANNEL() {
    return "443 ";
}

std::string ERR_NEEDMOREPARAMS(std::string nick, std::string arg) {
    return "461 " + nick + ": " + arg + ":Not enough parameters";
}

std::string ERR_ALREADYREGISTRED(std::string nick) {
    return "462 " + nick + ":You may not reregister";
}

std::string ERR_ERRONEUSNICKNAME() {
    return "462 ";
}

std::string ERR_PASSWDMISMATCH(std::string nick) {
    return "464 " + nick + ":Password incorrect";
}

std::string ERR_CHANNELISFULL(std::string nick) {
    return "471 " + nick + ":Cannot join channel (+l)";
}

std::string ERR_UNKNOWNMODE() {
    return "472 ";
}

std::string ERR_INVITEONLYCHAN() {
    return "473 ";
}

std::string ERR_NOPRIVILEGES() {
    return "481 ";
}

std::string ERR_CHANOPRIVSNEEDED() {
    return "482 ";
}

std::string ERR_UMODEUNKNOWNFLAG() {
    return "501 ";
}

std::string ERR_USERSDONTMATCH() {
    return "502 ";
}
