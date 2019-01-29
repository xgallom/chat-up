//
// Created by xgallom on 1/25/19.
//

#ifndef CHAT_UP_MESSAGEBODY_H
#define CHAT_UP_MESSAGEBODY_H

#include "Messaging/MessageType.h"

#include <cstddef>
#include <memory>
#include <Socket/ReadWriteSocket.h>

struct MessageBody {
    static const MessageType::Enum Type = MessageType::Invalid;
};

template<typename BodyType>
inline size_t bodySize(const BodyType *) noexcept
{
    static_assert(std::is_base_of<MessageBody, BodyType>::value, "Invalid type BodyType");

    return sizeof(BodyType);
}

template<typename BodyType>
inline void writeBody(ReadWriteSocket &socket, const BodyType &body) noexcept
{
    static_assert(std::is_base_of<MessageBody, BodyType>::value, "Invalid type BodyType");

    const auto buf = reinterpret_cast<const char *>(&body);

    socket.write(buf, bodySize<BodyType>(nullptr));
}

template<typename BodyType, typename ... Args>
inline std::unique_ptr<BodyType> createBody(Args &&... args) noexcept
{
    static_assert(std::is_base_of<MessageBody, BodyType>::value, "Invalid type BodyType");

    return std::make_unique<BodyType>(std::forward<Args>(args) ...);
}

std::unique_ptr<MessageBody> createBody(uint16_t type);

#endif //CHAT_UP_MESSAGEBODY_H
