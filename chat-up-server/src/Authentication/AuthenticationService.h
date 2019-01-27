//
// Created by xgallom on 1/27/19.
//

#ifndef CHAT_UP_AUTHENTICATIONSERVICE_H
#define CHAT_UP_AUTHENTICATIONSERVICE_H

#include <Messaging/Message.h>
#include <Messaging/MessageSender.h>
#include <Outcome.h>

class AuthenticationStorage;

class AuthenticationService {
	AuthenticationStorage &m_storage;

public:
	AuthenticationService() noexcept;

	Outcome::Enum run(MessageSender &sender, const Message &message);
};


#endif //CHAT_UP_AUTHENTICATIONSERVICE_H
