//
// Created by xgallom on 1/25/19.
//

#ifndef CHAT_UP_MESSAGEEXCEPTION_H
#define CHAT_UP_MESSAGEEXCEPTION_H

#include<stdexcept>

struct MessageException : public std::runtime_error {
    MessageException(const char message[]) : std::runtime_error(message) {}
};

#endif //CHAT_UP_MESSAGEEXCEPTION_H
