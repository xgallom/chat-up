//
// Created by xgallom on 1/26/19.
//

#include <Messaging/Messages/Handshake.h>
#include <Messaging/Version.h>
#include <Messaging/MessageException.h>
#include <Messaging/Messages/Authentication.h>
#include <iostream>
#include <Messaging/Messages/Content.h>
#include "ClientService.h"

ClientService::ClientService(ClientSocket &socket, const Broadcast &broadcast) noexcept
        : m_receiver(socket), m_sender(socket), m_broadcast(broadcast) {}

Outcome::Enum ClientService::run()
{
    const auto message = m_receiver.receiveMessage();

    if(message.isValid()) {
        switch(m_state) {
            case StateHandshaking:
                return runHandshaking(message);

            case StateAuthenticating: {
                const auto outcome = m_authenticationService.run(m_sender, message);

                if(outcome == Outcome::Success)
                    m_state = StateRunning;

                return outcome;
            }

            case StateRunning:
                return runRunning(message);
        }
    }

    return Outcome::Retry;
}

Outcome::Enum ClientService::runHandshaking(const Message &message)
{
    if(message.type() == MessageType::Handshake) {
        const auto &body = message.bodyAs<HandshakeMessageBody>();

        if(body.version.major == MessagingVersion::Major &&
           body.version.minor == MessagingVersion::Minor) {
            m_sender.sendMessage<HandshakeSuccessfulMessageBody>(Message::Create<HandshakeSuccessfulMessageBody>());

            m_state = StateAuthenticating;

            return Outcome::Success;
        }
        else {
            auto responseBody = createBody<HandshakeFailedMessageBody>();

            responseBody->version.major = MessagingVersion::Major;
            responseBody->version.minor = MessagingVersion::Minor;

            m_sender.sendMessage<HandshakeFailedMessageBody>(Message(std::move(responseBody)));
        }
    }

    return Outcome::Failed;
}

Outcome::Enum ClientService::runRunning(const Message &message)
{
    if(message.type() == MessageType::TextContent) {
        const auto &body = message.bodyAs<TextContentMessageBody>();

        std::cout << "Message from " << m_authenticationService.user().username << ": " << body.content.message
                  << std::endl;

        Message broadcastMessage(std::make_unique<TextContentMessageBody>(body));
        {
            std::lock_guard lockGuard(m_broadcast.second);

            for(auto &broadcastSender : m_broadcast.first)
                broadcastSender->m_sender.sendMessage<TextContentMessageBody>(broadcastMessage);
        }
    }

    return Outcome::Retry;
}
