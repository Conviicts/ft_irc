#pragma once

class UserMode {
    public:
        enum Flag {
            CREATOR,
            OPERATOR,
            USER
        };

        UserMode();
        UserMode(int mode);
        ~UserMode();

        bool canTalk() const;
    private:
        int        _mode;
};