//
// Created by xgallom on 1/26/19.
//

#ifndef CHAT_UP_CLIENTSERVICE_H
#define CHAT_UP_CLIENTSERVICE_H

#include "ClientSocket.h"
#include <Messaging/MessageReceiver.h>
#include <Messaging/MessageSender.h>

class ClientService {
	enum State {
		StateHandshaking,
		StateHandshakingWaiting,
		StateAuthenticating,
		StateAuthenticatingWaiting,
		StateRunning
	};

	MessageReceiver m_receiver;
	MessageSender m_sender;

	State m_state = StateHandshaking;

public:
	explicit ClientService(ClientSocket &socket) noexcept;

	bool run();

private:
	bool runHandshaking();
	bool runHandshakingWaiting();
	bool runAuthenticating();
	bool runAuthenticatingWaiting();
	bool runRunning();
};


#endif //CHAT_UP_CLIENTSERVICE_H
