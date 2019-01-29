//
// Created by xgallom on 1/23/19.
//

#include "Socket/Socket.h"
#include "Socket/SocketException.h"

extern "C" {
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
}

#include <iostream>

Socket::Socket(int socket) noexcept : m_socket(socket) {}

Socket::Socket()
{
    open();
}

Socket::~Socket()
{
    close();
}

Socket::Socket(Socket &&o) noexcept
{
    m_socket = o.m_socket;
    o.m_socket = 0;
}

void Socket::open()
{
    if(isOpen())
        return;

    std::cout << "Opening socket\n";

    m_socket = ::socket(AF_INET, SOCK_STREAM, 0);

    if(!IsValid(m_socket)) {
        m_socket = 0;
        throw SocketException("Socket creation failed");
    }
}

void Socket::close() noexcept
{
    if(isOpen()) {
        ::close(m_socket);
        m_socket = 0;
    }
}

bool Socket::isOpen() const noexcept
{
    return static_cast<bool>(m_socket);
}

int Socket::fd() const noexcept
{
    return m_socket;
}

bool Socket::IsValid(int socket) noexcept
{
    return socket >= 0;
}

