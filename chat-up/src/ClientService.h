//
// Created by xgallom on 1/26/19.
//

#ifndef CHAT_UP_CLIENTSERVICE_H
#define CHAT_UP_CLIENTSERVICE_H

#include "ClientSocket.h"
#include <Authentication/User.h>
#include <Messaging/MessageReceiver.h>
#include <Messaging/MessageSender.h>
#include <Outcome.h>
#include <Messaging/Messages/Content.h>
#include <vector>

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

    User m_user = User();
    std::vector<TextContentMessageBody> m_content = std::vector<TextContentMessageBody>();

public:
    explicit ClientService(ClientSocket &socket) noexcept;

    Outcome::Enum run();

private:
    Outcome::Enum runHandshaking();
    Outcome::Enum runHandshakingWaiting();
    Outcome::Enum runAuthenticating();
    Outcome::Enum runAuthenticatingWaiting();
    Outcome::Enum runRunning();
};


#endif //CHAT_UP_CLIENTSERVICE_H
