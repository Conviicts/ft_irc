#pragma once

class UserMode {
    public:
        enum Flag {
            CREATOR,
            OPERATOR, // chanop
            USER
        };

        UserMode();
        UserMode(Flag mode);
        ~UserMode();

        bool isChanOP() const;
        void setChanOP(bool op);
    private:
        Flag        _mode;
};