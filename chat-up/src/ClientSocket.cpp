//
// Created by xgallom on 1/24/19.
//

#include "ClientSocket.h"
#include <Socket/SocketException.h>

extern "C" {
#include <sys/socket.h>
#include <unistd.h>
}

#include <iostream>

void ClientSocket::connect(const SocketAddress &socketAddress)
{
    std::cout << "Connecting to " << socketAddress << std::endl;

    const auto address = socketAddress.address();

    if(::connect(fd(), reinterpret_cast<const sockaddr *>(&address), sizeof(address)))
        throw SocketException("Socket connection failed");
}
