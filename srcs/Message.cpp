#include "Message.hpp"

std::vector<std::string>    split(const std::string &str, char delim) {
    std::vector<std::string>    res;
    std::string                 tmp;
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == delim) {
            res.push_back(tmp);
            tmp.clear();
        } else
            tmp += str[i];
    }
    res.push_back(tmp);
    return res;
}

void eraseChar(std::string &str, const std::string &delim) {
    size_t pos = str.find(delim);
    if (pos != std::string::npos)
        str.erase(pos, delim.length());
}

Message::Message(const std::string &message) {
    std::vector<std::string> args = split(message, ' ');
    eraseChar(args[0], "\r");
    eraseChar(args[0], "\n");
    _command = args[0];
    for (size_t i = 1; i < args.size(); i++) {
        eraseChar(args[i], "\r");
        eraseChar(args[i], "\n");
        if (!args[i].empty() && args[i] != ":")
            _args.push_back(args[i]);
    }
}

Message::~Message() { }


const std::string                   &Message::command() const { return _command; }
const std::vector<std::string>      &Message::args() const { return _args; }