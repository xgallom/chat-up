//
// Created by xgallom on 1/24/19.
//

#ifndef CHAT_UP_CLIENTSOCKET_H
#define CHAT_UP_CLIENTSOCKET_H

#include <Socket/ReadWriteSocket.h>
#include <Socket/SocketAddress.h>

class ClientSocket : public ReadWriteSocket {
public:
    ClientSocket();

    void connect(const SocketAddress &socketAddress);
};

#endif //CHAT_UP_CLIENTSOCKET_H
