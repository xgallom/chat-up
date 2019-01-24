//
// Created by xgallom on 1/24/19.
//

#ifndef CHAT_UP_SERVERSOCKET_H
#define CHAT_UP_SERVERSOCKET_H

#include <Socket/Socket.h>
#include <Socket/SocketAddress.h>
#include "ClientSocket.h"

class ServerSocket : public Socket {
public:
	ServerSocket() = default;
	~ServerSocket() = default;

	ServerSocket(ServerSocket &&) noexcept = default;
	ServerSocket &operator =(ServerSocket &&) noexcept = default;

	void bind(const SocketAddress &socketAddress);
	void listen(int maxPending);
	ClientSocket accept();
};

#endif //CHAT_UP_SERVERSOCKET_H
