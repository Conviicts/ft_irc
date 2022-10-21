#include "IrcServer.hpp"
#include "Channel.hpp"
#include "Replies.hpp"

IrcServer::IrcServer(char **av) :
    _port(av[1]),
    _password(av[2]),
    _state(STARTED),
    _init(false)
{
	_userCommands["PASS"] = &IrcServer::PASS;
	_userCommands["NICK"] = &IrcServer::NICK;
	_userCommands["USER"] = &IrcServer::USER;
	_userCommands["QUIT"] = &IrcServer::QUIT;
	_userCommands["PRIVMSG"] = &IrcServer::PRIVMSG;
	_userCommands["JOIN"] = &IrcServer::JOIN;
    _userCommands["OPER"] = &IrcServer::OPER;
    _userCommands["MODE"] = &IrcServer::MODE;
    _userCommands["PART"] = &IrcServer::PART;
    _userCommands["TOPIC"] = &IrcServer::TOPIC;
    _userCommands["NAMES"] = &IrcServer::NAMES;
    _userCommands["LIST"] = &IrcServer::LIST;
    _userCommands["INVITE"] = &IrcServer::INVITE;
    _userCommands["KICK"] = &IrcServer::KICK;
    _userCommands["NOTICE"] = &IrcServer::NOTICE;
    _userCommands["WHO"] = &IrcServer::WHO;
    _userCommands["KILL"] = &IrcServer::KILL;
    _userCommands["kill"] = &IrcServer::KILL;
    _userCommands["PING"] = &IrcServer::PING;
}

IrcServer::~IrcServer() { }

IrcServer::State    IrcServer::state() const { return _state; }

void                IrcServer::logger(int type, const std::string msg, const std::string user = "", std::vector<std::string> args = std::vector<std::string>()) {

    const time_t t = time(NULL);
    char buff[256];
    strftime(buff, 256, "%d-%m-%Y %H:%M:%S", localtime(&t));
    std::cout << "\033[1;31m" << buff << std::setw(5) << "\033[0m ";
    if (type == 1)
        std::cout << "\033[1;32m" << "[SERVER]:" << "\033[0m " << msg;
    else if (type == 2) {
        std::cout << "\033[1;33m" << "[MESSAGE]:" << "\033[0m ";
        std::cout << "[";
        std::cout << std::setw(10) << user;
        std::cout << "] " << msg;
        for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it)
            std::cout << " " << *it;
    }
    std::cout << std::endl;
}

void                IrcServer::run() {
    std::cout << "------------------------------------------------     " << std::endl;
    std::cout << "______________________ .______________________       " << std::endl;
    std::cout << "\\_   _____/\\__    ___/ |   \\______   \\_   ___ \\ " << std::endl;
    std::cout << " |    __)    |    |    |   ||       _/    \\  \\/    " << std::endl;
    std::cout << " |     \\     |    |    |   ||    |   \\     \\____  " << std::endl;
    std::cout << " \\___  /     |____|____|___||____|_  /\\______  /   " << std::endl;
    std::cout << "     \\/          /_____/           \\/        \\/   " << std::endl;
    std::cout << "------------------------------------------------     " << std::endl;
    logger(1, "Listening on port " + _port);
    if (!_init) {
        _server.listen(_port);
        _server.setMaxConnections(20);
        _init = true;
    }
    while (_state == STARTED) {
        try {
            _server.select();
        } catch (std::exception &e) {
            _state = CLOSED;
            return ;
        }
		flushZombies();
        TCP::TCPSocket *newSocket;
        while ((newSocket = _server.getNextNewConnection())) {
            User *u = new User(newSocket);
            logger(1, "New client connected: " + newSocket->host());
            _network.add(u);
        }
        TCP::TCPSocket *socket;
        while ((socket = _server.getNextPendingConnection()) && _state == STARTED) {
            try {
                socket->flush();
                std::string line;
                while (socket->canRead()) {
                    if (!socket->read(line)) {
                        disconnect(socket, "Remote host closed the connection.");
                        continue ;
                    }
                    if (socket->bufferSize() > 2048) {
                        disconnect(socket, "Received too much data.");
                        continue ;
                    }
                    if (line.empty())
                        continue ;
                    Message msg = Message(line);
                    execute(_network.getUserBySocket(socket), msg);
                }
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
                disconnect(socket, e.what());
                continue ;
            }
        }
    }
}

void                IrcServer::execute(TCP::BasicConnection *c, Message message) {
	User &user = *static_cast<User*>(c);
	userCommands::const_iterator i = _userCommands.find(message.command());
	if (i == _userCommands.end()) {
        logger(1, "UNKNOW COMMAND: " + message.command());
        return ;
    }
    logger(2, message.command(), user.nickname().size() ? user.nickname() : "*", message.args());
	(this->*(i->second))(user, message);
}

void                IrcServer::flushZombies() {
	TCP::BasicConnection *z;

	while ((z = _network.nextZombie())) {
		try {
            z->socket()->flush();
        } catch(std::exception &e) {}
		_server.disconnect(z->socket());
		delete z;
	}
}

void                IrcServer::disconnect(TCP::TCPSocket *socket, const std::string &reason) throw() {
    TCP::BasicConnection *c = _network.getUserBySocket(socket);
	disconnect(*static_cast<User *>(c), reason);
}

void                IrcServer::disconnect(User &user, const std::string &reason, bool notifyUser) throw() {
    if (user.channelsCount()) {
        const Network::Channels &channels = _network.channels();
        Network::Channels::const_iterator it = _network.channels().begin();
        while (it != channels.end()) {
            Channel *channel = it->second;
            it++;
            if (channel->getUser(&user)) {
                channel->delUser(&user);
                if (channel->clientSize() == 0) {
                    _network.remove(channel);
                } else {
                    channel->broadcast2("QUIT " + channel->name() + " " + user.username() + " " + reason);
                }
            }
        }
    }
    logger(1, "Disconnected: " + user.socket()->ip() + " fd: " + itoa(user.socket()->fd()));        
    if (notifyUser) {
        logger(1, "Reason: " + reason);        
        std::string quit_reason;
        quit_reason =  reason.empty() ? "(Client Quit) ": reason;
        user.write(quit_reason);
    }
    _network.remove(&user);
	_network.newZombie(&user);
}
