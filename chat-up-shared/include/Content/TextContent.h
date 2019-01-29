//
// Created by xgallom on 1/29/19.
//

#ifndef CHAT_UP_TEXTCONTENT_H
#define CHAT_UP_TEXTCONTENT_H

#include <array>

struct TextContent {
    std::array<char, 16> user;
    std::array<char, 1008> message;
};

#endif //CHAT_UP_TEXTCONTENT_H
