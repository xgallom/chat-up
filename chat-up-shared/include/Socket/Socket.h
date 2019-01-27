//
// Created by xgallom on 1/23/19.
//

#ifndef CHAT_UP_SOCKET_H
#define CHAT_UP_SOCKET_H

class Socket {
    int m_socket = 0;

protected:
    explicit Socket(int socket) noexcept;

public:
    Socket();
    ~Socket();

    Socket(const Socket &) noexcept = delete;
    Socket &operator=(const Socket &) noexcept = delete;

    Socket(Socket &&o) noexcept;
    Socket &operator=(Socket &&) noexcept = default;

    void open();
    void close() noexcept;

    bool isOpen() const noexcept;

protected:
    int fd() const noexcept;

    static bool IsValid(int socket) noexcept;
};

#endif //CHAT_UP_SOCKET_H
