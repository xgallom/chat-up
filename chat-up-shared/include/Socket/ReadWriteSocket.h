//
// Created by xgallom on 1/25/19.
//

#ifndef CHAT_UP_READWRITESOCKET_H
#define CHAT_UP_READWRITESOCKET_H

#include "Socket.h"
#include <cstddef>
#include <string>
#include <mutex>

class ReadWriteSocket : public Socket {
    std::mutex m_accessMutex = std::mutex();

protected:
    explicit ReadWriteSocket(int socket) noexcept;

public:
    ReadWriteSocket() = default;

    int readByte() noexcept;
    void readWalk(char *&buf, size_t &remaining) noexcept;

    void writeByte(char data) noexcept;
    void write(const char data[]) noexcept;
    void write(const char *data, size_t length) noexcept;
    void write(const std::string &data) noexcept;
};

#endif //CHAT_UP_READWRITESOCKET_H
