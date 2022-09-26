#include "UserMode.hpp"

UserMode::UserMode(Flag mode) : _mode(mode) {}
UserMode::UserMode() {}

UserMode::~UserMode() {}

bool UserMode::isChanOP() const {
	return (_mode >= USER ? false : true);
}