//
// Created by xgallom on 1/24/19.
//

#include "Messaging/Message.h"

Message::Message(Message &&o) noexcept
{
	std::swap(header, o.header);
	std::swap(body, o.body);
}

uint16_t Message::type() const noexcept
{
	return header.type;
}

bool Message::isValid() const noexcept
{
	return static_cast<bool>(header.type);
}
