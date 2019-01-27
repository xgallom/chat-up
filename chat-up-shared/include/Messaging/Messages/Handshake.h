//
// Created by xgallom on 1/26/19.
//

#ifndef CHAT_UP_TESTMESSAGE_H
#define CHAT_UP_TESTMESSAGE_H

#include "Messaging/MessageBody.h"
#include <cstdint>

struct HandshakeMessageBody : public MessageBody {
	static const MessageType::Enum Type = MessageType::Handshake;

	struct {
		uint8_t major, minor;
	} version;
};

struct HandshakeSuccessfulMessageBody : public MessageBody {
	static const MessageType::Enum Type = MessageType::HandshakeSuccessful;
};

struct HandshakeFailedMessageBody : public MessageBody {
	static const MessageType::Enum Type = MessageType::HandshakeFailed;

	struct {
		uint8_t major, minor;
	} version;
};

#endif //CHAT_UP_TESTMESSAGE_H
