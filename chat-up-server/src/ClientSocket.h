//
// Created by xgallom on 1/24/19.
//

#ifndef CHAT_UP_CLIENTSOCKET_H
#define CHAT_UP_CLIENTSOCKET_H

#include <Socket/ReadWriteSocket.h>

class ClientSocket : public ReadWriteSocket {
public:
	explicit ClientSocket(int socket) noexcept;

	ClientSocket(ClientSocket &&) noexcept = default;
	ClientSocket &operator =(ClientSocket &&) noexcept = default;
};

#endif //CHAT_UP_CLIENTSOCKET_H
