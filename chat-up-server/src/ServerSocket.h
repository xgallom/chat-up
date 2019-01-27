//
// Created by xgallom on 1/24/19.
//

#ifndef CHAT_UP_SERVERSOCKET_H
#define CHAT_UP_SERVERSOCKET_H

#include <Socket/Socket.h>
#include <Socket/SocketAddress.h>
#include "ClientSocket.h"

#include <optional>

class ServerSocket : public Socket {
public:
	int maxConcurrentConnections = 0;

public:
	ServerSocket();

	ServerSocket(ServerSocket &&) noexcept;
	ServerSocket &operator =(ServerSocket &&) noexcept = default;

	void bind(const SocketAddress &socketAddress);
	void listen(int maxPending);
	std::optional<ClientSocket> accept();
};

#endif //CHAT_UP_SERVERSOCKET_H
