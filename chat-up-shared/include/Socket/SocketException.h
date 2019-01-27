//
// Created by xgallom on 1/24/19.
//

#ifndef CHAT_UP_SOCKETEXCEPTION_H
#define CHAT_UP_SOCKETEXCEPTION_H

#include <stdexcept>

struct SocketException : public std::runtime_error {
    explicit SocketException(const char msg[]) noexcept;
};

#endif //CHAT_UP_SOCKETEXCEPTION_H
