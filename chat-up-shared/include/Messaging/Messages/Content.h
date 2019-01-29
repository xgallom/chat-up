//
// Created by xgallom on 1/29/19.
//

#ifndef CHAT_UP_CONTENT_H
#define CHAT_UP_CONTENT_H

#include "Messaging/MessageBody.h"
#include "Content/TextContent.h"

struct TextContentMessageBody : public MessageBody {
    static const MessageType::Enum Type = MessageType::TextContent;

    TextContent content;
};

#endif //CHAT_UP_CONTENT_H
