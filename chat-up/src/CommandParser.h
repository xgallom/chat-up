//
// Created by xgallom on 1/29/19.
//

#ifndef CHAT_UP_COMMANDPARSER_H
#define CHAT_UP_COMMANDPARSER_H

#include <Messaging/MessageBody.h>
#include <Messaging/MessageSender.h>

#include <functional>
#include <memory>
#include <sstream>

namespace CommandParser
{
    bool parse(MessageSender &sender, std::stringstream command);
}


#endif //CHAT_UP_COMMANDPARSER_H
