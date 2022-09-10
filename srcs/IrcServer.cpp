#include "IrcServer.hpp"

IrcServer::IrcServer() : _state(STARTED), _init(false) {
	_userCommands["PASS\n"] = &IrcServer::PASS;

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
                    execute(_network.getUserBySocket(socket), line);
                }
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
                disconnect(socket, e.what());
                continue ;
            }
        }
    }
}

void            IrcServer::execute(TCP::BasicConnection *c, const std::string &message) {
    (void)c;
    (void)message;
	User &user = *static_cast<User*>(c);
	userCommands::const_iterator i = _userCommands.find(message);
	if (i == _userCommands.end()) {
		std::cout << "unknow command: [" << message << "]" << std::endl;
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

int IrcServer::PASS(User &u, const std::string &m) {
	(void)u;
	(void)m;
    return 1;
}