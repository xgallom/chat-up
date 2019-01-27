//
// Created by xgallom on 1/27/19.
//

#include "Messaging/MessageBody.h"
#include "Messaging/MessageType.h"
#include "Messaging/MessageException.h"
#include <array>
#include <functional>
#include <Messaging/Messages/Handshake.h>
#include <Messaging/Messages/Authentication.h>
#include <sstream>

std::unique_ptr<MessageBody> createBody(uint16_t type)
{
    static const std::array<std::function<std::unique_ptr<MessageBody>(void)>, MessageType::Size> bodyCreators = {
            createBody<MessageBody>,

            createBody<HandshakeMessageBody>,
            createBody<HandshakeSuccessfulMessageBody>,
            createBody<HandshakeFailedMessageBody>,

            createBody<AuthenticationMessageBody>,
            createBody<AuthenticationSuccessfulBody>,
            createBody<AuthenticationFailedBody>
    };

    if(type < MessageType::Size)
        return bodyCreators[type]();

    std::stringstream ss;
    ss << "Invalid message type received: " << type;
    throw MessageException(ss.str().c_str());
}

