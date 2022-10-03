#include "UserMode.hpp"

UserMode::UserMode(Flag mode) : _mode(mode) {}
UserMode::UserMode() {}

UserMode::~UserMode() {}

bool UserMode::isChanOP() const {
	return (_mode >= USER ? false : true);
}

void UserMode::setChanOP(bool op) {
	if (op)
		_mode = OPERATOR;
	else
		_mode = USER;
}
