//
// Created by xgallom on 1/26/19.
//

#ifndef CHAT_UP_AUTHENTICATION_H
#define CHAT_UP_AUTHENTICATION_H

#include "Messaging/MessageBody.h"
#include "Authentication/User.h"

struct AuthenticationMessageBody : public MessageBody {
	static const MessageType::Enum Type = MessageType::Authentication;

	User user = User();
};

struct AuthenticationSuccessfulBody : public MessageBody {
	static const MessageType::Enum Type = MessageType::AuthenticationSuccessful;
};

struct AuthenticationFailedBody : public MessageBody {
	static const MessageType::Enum Type = MessageType::AuthenticationFailed;
};

#endif //CHAT_UP_AUTHENTICATION_H
