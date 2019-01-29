//
// Created by xgallom on 1/24/19.
//

#include "ServerSocket.h"
#include <Socket/SocketException.h>

extern "C" {
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
}

#include <iostream>

ServerSocket::ServerSocket()
{
    const int options = 1;

    if(setsockopt(fd(), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &options, sizeof(options))) {
        close();

        throw SocketException("Socket configuration failed");
    }

    const int flags = fcntl(fd(), F_GETFL, 0);

    if(flags == -1 || fcntl(fd(), F_SETFL, flags | O_NONBLOCK)) {
        close();

        throw SocketException("Making socket non-blocking failed");
    }
}

ServerSocket::ServerSocket(ServerSocket &&o) noexcept : Socket(std::move(o))
{
    maxConcurrentConnections = o.maxConcurrentConnections;
}

void ServerSocket::bind(const SocketAddress &socketAddress)
{
    std::cout << "Binding server socket to address " << socketAddress << std::endl;

    const auto address = socketAddress.address();

    if(::bind(fd(), reinterpret_cast<const sockaddr *>(&address), sizeof(address)))
        throw SocketException("Socket binding failed");
}

void ServerSocket::listen(int maxPending)
{
    std::cout << "Server socket listening to maximum " << maxPending << " pending connections\n";

    if(::listen(fd(), maxPending))
        throw SocketException("Listen failed");
}

std::unique_ptr<ClientSocket> ServerSocket::accept()
{
    int clientSocket = ::accept(fd(), nullptr, nullptr);

    if(clientSocket >= 0)
        return std::make_unique<ClientSocket>(clientSocket);
    else if(errno == EWOULDBLOCK)
        return nullptr;

    throw SocketException("Server socket accept failed");
}
