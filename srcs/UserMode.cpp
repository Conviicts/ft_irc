#include "UserMode.hpp"

UserMode::UserMode(int mode) : _mode(mode) {}
UserMode::UserMode() {}

UserMode::~UserMode() {}

bool UserMode::canTalk() const {
	return (0);
}