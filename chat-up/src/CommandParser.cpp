//
// Created by xgallom on 1/29/19.
//

#include "CommandParser.h"
#include <Messaging/Messages/Authentication.h>
#include <sstream>

namespace CommandParser
{
    enum CommandType {
        Register = 0,

        Size
    };

    static bool registerHandler(MessageSender &sender, std::stringstream &command)
    {
        auto body = createBody<RegisterMessageBody>();
        const auto maxLen = body->user.username.size();

        std::string str;
        str.reserve(maxLen);

        std::getline(command, str, ' ');

        if(!str.length())
            return false;

        copy(body->user.username, std::string_view(str.c_str(), std::min(maxLen, str.length())));

        std::getline(command, str, ' ');

        if(!str.length())
            return false;

        copy(body->user.password, std::string_view(str.c_str(), std::min(maxLen, str.length())));

        sender.sendMessage<RegisterMessageBody>(Message(std::move(body)));

        return true;
    }

    bool parse(MessageSender &sender, std::stringstream command)
    {
        std::array<const char[10], Size> commandKeys = {
                "/register"
        };

        static const std::array<std::function<bool(MessageSender &, std::stringstream &command)>, Size> commandHandlers = {
                registerHandler
        };

        std::string cmd;

        std::getline(command, cmd, ' ');

        const auto i = std::find(commandKeys.cbegin(), commandKeys.cend(), cmd);

        if(i != commandKeys.cend())
            return commandHandlers[static_cast<size_t>(std::distance(commandKeys.cbegin(), i))](sender, command);
        else
            return false;
    }
}
