//
// Created by xgallom on 1/25/19.
//

#ifndef CHAT_UP_MESSAGEREADER_H
#define CHAT_UP_MESSAGEREADER_H

#include "Message.h"

class ReadWriteSocket;

class MessageReceiver {
    enum State {
        ReadingOpener,
        ReadingHeader,
        ReadingBody
    };

    ReadWriteSocket &m_socket;
    State m_state = ReadingOpener;
    Message m_currentMessage = Message();
    size_t m_remaining = 1;

public:
    explicit MessageReceiver(ReadWriteSocket &socket) noexcept;

    Message receiveMessage();

private:
    Message readOpener();
    Message readHeader();
    Message readBody() noexcept;
};

#endif //CHAT_UP_MESSAGERECEIVER_H
