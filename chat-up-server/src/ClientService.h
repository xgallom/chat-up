//
// Created by xgallom on 1/26/19.
//

#ifndef CHAT_UP_CLIENTSERVICE_H
#define CHAT_UP_CLIENTSERVICE_H

#include <Messaging/MessageReceiver.h>
#include <Messaging/MessageSender.h>
#include "ClientSocket.h"

class ClientService {
	enum State {
		StateHandshaking,
		StateAuthenticating,
		StateRunning
	};

	MessageReceiver m_receiver;
	MessageSender m_sender;

	State m_state = StateHandshaking;

public:
	explicit ClientService(ClientSocket &socket) noexcept;

	void run();

private:
	void runHandshaking(const Message &message);
	void runAuthenticating(const Message &message);
	void runRunning(const Message &message);
};


#endif //CHAT_UP_CLIENTSERVICE_H
