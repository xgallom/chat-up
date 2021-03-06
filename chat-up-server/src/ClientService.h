//
// Created by xgallom on 1/26/19.
//

#ifndef CHAT_UP_CLIENTSERVICE_H
#define CHAT_UP_CLIENTSERVICE_H

#include "ClientSocket.h"
#include <Messaging/MessageReceiver.h>
#include <functional>
#include "Authentication/AuthenticationService.h"

class ClientService {
public:
    using Broadcast = std::pair<std::vector<ClientService *> &, std::mutex &>;

private:
    enum State {
        StateHandshaking,
        StateAuthenticating,
        StateRunning
    };

    MessageReceiver m_receiver;
    MessageSender m_sender;

    State m_state = StateHandshaking;

    AuthenticationService m_authenticationService = AuthenticationService();

    const Broadcast m_broadcast;

public:
    explicit ClientService(ClientSocket &socket, const Broadcast &broadcast) noexcept;

    Outcome::Enum run();

private:
    Outcome::Enum runHandshaking(const Message &message);
    Outcome::Enum runRunning(const Message &message);
};


#endif //CHAT_UP_CLIENTSERVICE_H
