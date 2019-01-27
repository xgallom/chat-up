//
// Created by xgallom on 1/24/19.
//

#ifndef CHAT_UP_MESSAGE_H
#define CHAT_UP_MESSAGE_H

#include "MessageHeader.h"
#include "MessageBody.h"

#include <string>

struct Message {
    MessageHeader header = MessageHeader();
    std::unique_ptr<MessageBody> body = nullptr;

    Message() noexcept = default;
    template<typename BodyType>
    explicit Message(std::unique_ptr<BodyType> &&a_body) noexcept
            : header(static_cast<uint16_t>(bodySize(a_body.get())),
                     static_cast<uint16_t>(BodyType::Type))
              , body(std::move(a_body)) {}

    Message(const Message &) = delete;
    Message &operator=(const Message &) = delete;

    Message(Message &&o) noexcept;
    Message &operator=(Message &&) noexcept = default;

    uint16_t type() const noexcept;

    template<typename BodyType>
    BodyType &bodyAs() const noexcept
    {
        return static_cast<BodyType &>(*body);
    }

    bool isValid() const noexcept;

    template<typename BodyType>
    static Message Create()
    {
        return Message(createBody<BodyType>());
    }
};


#endif //CHAT_UP_MESSAGE_H
