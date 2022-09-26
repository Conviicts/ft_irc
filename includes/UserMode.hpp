#pragma once

class UserMode {
    public:
        enum Flag {
            CREATOR,
            OPERATOR,
            USER
        };

        UserMode();
        UserMode(Flag mode);
        ~UserMode();

        bool isChanOP() const;
    private:
        Flag        _mode;
};