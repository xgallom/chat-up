//
// Created by xgallom on 1/27/19.
//

#include "AuthenticationService.h"
#include "AuthenticationStorage.h"
#include <Messaging/Messages/Authentication.h>
#include <iostream>

AuthenticationService::AuthenticationService() noexcept : m_storage(AuthenticationStorage::Instance()) {}

Outcome::Enum AuthenticationService::run(MessageSender &sender, const Message &message)
{
    if(message.type() == MessageType::Authentication) {
        const auto &body = message.bodyAs<AuthenticationMessageBody>();

        std::cout << "Authentication with\n  Username: " << body.user.username << "\n  Password: " << body.user.password
                  << std::endl;

        if(m_storage.contains(body.user)) {
            m_user = body.user;

            sender.sendMessage<AuthenticationSuccessfulBody>(Message::Create<AuthenticationSuccessfulBody>());

            return Outcome::Success;
        }
        else {
            sender.sendMessage<AuthenticationFailedBody>(Message::Create<AuthenticationFailedBody>());

            return Outcome::Retry;
        }
    }

    return Outcome::Failed;
}

bool AuthenticationService::registerUser(const User &user)
{
    return m_storage.insert(user);
}

User AuthenticationService::user() const noexcept
{
    return m_user;
}
