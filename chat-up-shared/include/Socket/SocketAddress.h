//
// Created by xgallom on 1/23/19.
//

#ifndef CHAT_UP_SOCKETADDRESS_H
#define CHAT_UP_SOCKETADDRESS_H

#include "IpAddress.h"

class SocketAddress {
	sockaddr_in m_address;

public:
	constexpr SocketAddress(const IpAddress ipAddress, uint16_t port) noexcept :
			m_address{
					AF_INET,
					port,
					{ipAddress.address},
					{}
			} {}

	sockaddr_in address() const noexcept;

	static constexpr uint16_t DefaultPort() noexcept { return 8080; }
};

std::ostream &operator<<(std::ostream &os, const SocketAddress &socketAddress);

#endif //CHAT_UP_SOCKETADDRESS_H
