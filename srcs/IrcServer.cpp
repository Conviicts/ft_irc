#include "IrcServer.hpp"

IrcServer::IrcServer() : _state(STARTED), _init(false) {
	_userCommands["PASS"] = &IrcServer::PASS;
	_userCommands["NICK"] = &IrcServer::NICK;
	_userCommands["USER"] = &IrcServer::USER;

}

IrcServer::~IrcServer() { }

IrcServer::State    IrcServer::state() const { return _state; }

void                IrcServer::run() {
    if (!_init) {
        _server.listen("10001");
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
            std::cout << "New client connected: " << newSocket->ip() << std::endl;
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

void            IrcServer::execute(TCP::BasicConnection *c, Message message) {
	User &user = *static_cast<User*>(c);
    std::cout << "(" << message.command() << ")" << std::endl;
	userCommands::const_iterator i = _userCommands.find(message.command());
	if (i == _userCommands.end()) {
		std::cout << "UNKNOWN COMMAND: [" << message.command() << "]" << std::endl;
		std::cout << ">\targs size: [" << message.args().size() << "]" << std::endl;
        return ;
    }
	int status = (this->*(i->second))(user, message);
    std::cout << "status: " << status << std::endl;
}

void            IrcServer::flushZombies() {
	TCP::BasicConnection *z;

	while ((z = _network.nextZombie()))
	{
		try {
            z->socket()->flush();
        } catch(std::exception &e) {}
		_server.disconnect(z->socket());
		delete z;
	}
}

void                IrcServer::disconnect(TCP::TCPSocket *socket, const std::string &reason) throw() {
    TCP::BasicConnection *c = _network.getUserBySocket(socket);
    std::cout << c->socket()->fd() << std::endl;
	disconnect(*static_cast<User *>(c), reason);
}

void                IrcServer::disconnect(User &user, const std::string &reason, bool notifyUser) throw() {
    (void)notifyUser;
    std::cout << "Disconnected: " << user.socket()->ip() << " fd: " << user.socket()->fd() << "\nReason: " << reason << std::endl;
    _network.remove(&user);
	_network.newZombie(&user);
}

int IrcServer::PASS(User &u, Message msg) {
    (void)msg;
    // if (msg.args().size() != 2)
    //     return u.reply(u, 461, msg.args());
    u.setState(1);
    return (1);
}

int IrcServer::NICK(User &u, Message msg) {
    u.setState(1); //TODO: remove !!!
    if (!msg.args().size())
        return u.reply(u, 431, msg.args());
    if (msg.args().size() != 1)
        return u.reply(u, 461, msg.args());
    u.setNickname(msg.args()[0]);
    std::cout << "nickname: " << u.nickname() << std::endl;
    return (1);
}

int IrcServer::USER(User &u, Message msg) {
    std::cout << msg.args().size() << std::endl;
    if (msg.args().size() != 4)
        return u.reply(u, 461, msg.args());
    if (u.state() != 1)
        return u.reply(u, 462, std::vector<std::string>());
    u.setUsername(msg.args()[0]);
    u.setRealname(msg.args()[3]);
    return u.reply(u, 0, msg.args());

    return (1);
}