#pragma once
#include <vector>
#include <iostream>

class Message {
    public:

        Message(const std::string &message);
        virtual ~Message();

        const std::string                   &command() const;
        const std::vector<std::string>      &args() const;
    private:
        std::string                 _command;
        std::vector<std::string>    _args;

};