//
// Created by xgallom on 1/25/19.
//

#ifndef CHAT_UP_MESSAGEHEADER_H
#define CHAT_UP_MESSAGEHEADER_H

#include "MessageType.h"
#include <cstdint>

struct MessageHeader {
	uint16_t length = 0;
	uint16_t type = MessageType::Invalid;

	MessageHeader() noexcept = default;
	explicit MessageHeader(uint16_t a_length, uint16_t a_type) noexcept : length(a_length), type(a_type) {}
};

#endif //CHAT_UP_MESSAGEHEADER_H
