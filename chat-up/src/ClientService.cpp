//
// Created by xgallom on 1/26/19.
//

#include <Messaging/Messages/Handshake.h>
#include <Messaging/Messages/Authentication.h>
#include <Messaging/Version.h>
#include "ClientService.h"
#include "Ui.h"
#include <iostream>
#include <Messaging/Messages/Content.h>
#include <thread>
#include <cstring>
#include <algorithm>

ClientService::ClientService(ClientSocket &socket) noexcept : m_receiver(socket), m_sender(socket) {}

Outcome::Enum ClientService::run()
{
    switch(m_state) {
        case StateHandshaking:
            return runHandshaking();

        case StateHandshakingWaiting:
            return runHandshakingWaiting();

        case StateAuthenticating:
            return runAuthenticating();

        case StateAuthenticatingWaiting:
            return runAuthenticatingWaiting();

        case StateRunning:
            return runRunning();
    }

    std::cerr << "Client service has invalid state\n";

    return Outcome::Failed;
}

Outcome::Enum ClientService::runHandshaking()
{
    auto body = createBody<HandshakeMessageBody>();

    body->version.major = MessagingVersion::Major;
    body->version.minor = MessagingVersion::Minor;

    Ui::Log() << "Handshaking with " << static_cast<int>(body->version.major) << "."
              << static_cast<int>(body->version.minor) << '\n';

    m_sender.sendMessage<HandshakeMessageBody>(Message(std::move(body)));

    m_state = StateHandshakingWaiting;

    return Outcome::Success;
}

Outcome::Enum ClientService::runHandshakingWaiting()
{
    const auto message = m_receiver.receiveMessage();

    if(message.isValid()) {
        switch(message.type()) {
            case MessageType::HandshakeSuccessful:
                Ui::Log() << "Handshake successful\n";

                m_state = StateAuthenticating;

                return Outcome::Success;

            case MessageType::HandshakeFailed: {
                const auto &body = message.bodyAs<HandshakeFailedMessageBody>();

                Ui::Log() << "Handshake failed: Server runs version " << static_cast<int>(body.version.major) << "."
                          << static_cast<int>(body.version.minor) << '\n';

                return Outcome::Failed;
            }

            default:
                Ui::Log() << "Invalid handshake response: " << message.type() << '\n';

                return Outcome::Failed;
        }
    }

    return Outcome::Retry;
}

Outcome::Enum ClientService::runAuthenticating()
{
    auto body = createBody<AuthenticationMessageBody>();

    static auto tries = 0;

    Ui::MessageLevel accent = tries ? Ui::MessageError : Ui::MessageInfo;
    const char *title = tries++ ?  "Login incorrect, try again" : "Please login";

    copy(body->user.username, Ui::PopupInput("Username: ", body->user.username.size() - 1, title, false, accent));
    copy(body->user.password, Ui::PopupInput("Password: ", body->user.password.size() - 1, title, true, accent));

    m_user = body->user;

    m_sender.sendMessage<AuthenticationMessageBody>(Message(std::move(body)));

    m_state = StateAuthenticatingWaiting;

    return Outcome::Success;
}

Outcome::Enum ClientService::runAuthenticatingWaiting()
{
    const auto message = m_receiver.receiveMessage();

    if(message.isValid()) {
        switch(message.type()) {
            case MessageType::AuthenticationSuccessful:
                Ui::Log() << "Authentication successful\n";

                m_state = StateRunning;

                return Outcome::Success;

            case MessageType::AuthenticationFailed:
                Ui::Log() << "Authentication failed\n";

                m_state = StateAuthenticating;

                break;

            default:
                Ui::Log() << "Invalid handshake response: " << message.type() << '\n';

                return Outcome::Failed;
        }
    }

    return Outcome::Retry;
}

Outcome::Enum ClientService::runRunning()
{
    static auto body = createBody<TextContentMessageBody>();;

    auto chatWindow  = Ui::Window::FlatHorizontal(Ui::AlignStart, -5, "Messages", Ui::MessageInfo);
    auto inputWindow = Ui::Window::FlatHorizontal(Ui::AlignEnd, 3, "Enter message:", Ui::MessagePrimary);

    {
        chatWindow.setTextLevel(chatWindow.messageLevel);

        int n = 0;
        for(const auto &content : m_content) {
            chatWindow.move(0, n++);

            chatWindow.print(std::string_view(content.content.user.cbegin(), content.content.user.size()));
            chatWindow.print(": ");
            chatWindow.print(std::string_view(content.content.message.cbegin(), content.content.message.size()));
        }

        chatWindow.refresh();
    }

    inputWindow.move(1, 1);
    inputWindow.setTextLevel(inputWindow.messageLevel);
    inputWindow.print(body->content.message.cbegin());

    std::string message(body->content.message.cbegin());

    Ui::setCursor(true);

    for(;;) {
        if(!m_receiver.isOpen()) {
            Ui::setCursor(false);

            return Outcome::Success;
        }

        if(const auto receivedMessage = m_receiver.receiveMessage(); receivedMessage.isValid()) {
            const auto &receivedBody = receivedMessage.bodyAs<TextContentMessageBody>();

            if(static_cast<int>(m_content.size()) < chatWindow.height)
                m_content.push_back(receivedBody);
            else {
                std::rotate(m_content.begin(), m_content.begin() + 1, m_content.end());
                m_content.back() = receivedBody;
            }

            copy(body->content.message, message);

            break;
        }

        if(inputWindow.readStringAsync(message, static_cast<size_t>(inputWindow.width - 2)) && message.size()) {
            copy(body->content.message, message);
            body->content.user = m_user.username;

            m_sender.sendMessage<TextContentMessageBody>(Message(std::make_unique<TextContentMessageBody>(*body)));

            *body = TextContentMessageBody();

            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    Ui::setCursor(false);

    return Outcome::Retry;
}
