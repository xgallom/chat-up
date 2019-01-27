//
// Created by xgallom on 1/26/19.
//

#ifndef CHAT_UP_AUTHENTICATION_H
#define CHAT_UP_AUTHENTICATION_H

#include "Messaging/MessageBody.h"

struct AuthenticationMessageBody : public MessageBody {
	static const MessageType::Enum Type = MessageType::Authentication;

	char
			username[16] = {},
			password[16] = {};
};

struct AuthenticationSuccessfulBody : public MessageBody {
	static const MessageType::Enum Type = MessageType::AuthenticationSuccessful;
};

struct AuthenticationFailedBody : public MessageBody {
	static const MessageType::Enum Type = MessageType::AuthenticationFailed;
};

#endif //CHAT_UP_AUTHENTICATION_H
