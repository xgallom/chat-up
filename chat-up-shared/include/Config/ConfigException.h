//
// Created by xgallom on 1/23/19.
//

#ifndef CHAT_UP_CONFIGEXCEPTION_H
#define CHAT_UP_CONFIGEXCEPTION_H

#include <stdexcept>

struct ConfigException : public std::logic_error {
	explicit ConfigException(const char msg[]) noexcept;
};

#endif //CHAT_UP_CONFIGEXCEPTION_H
