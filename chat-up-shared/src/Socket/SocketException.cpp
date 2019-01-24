//
// Created by xgallom on 1/24/19.
//

#include "Socket/SocketException.h"

SocketException::SocketException(const char msg[]) noexcept : std::runtime_error(msg) {}
