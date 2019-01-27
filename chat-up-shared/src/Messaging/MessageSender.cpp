//
// Created by xgallom on 1/26/19.
//

#include "Messaging/MessageSender.h"
#include "Socket/ReadWriteSocket.h"
#include "Messaging/MessageOpener.h"

MessageSender::MessageSender(ReadWriteSocket &socket) noexcept : m_socket(socket) {}
