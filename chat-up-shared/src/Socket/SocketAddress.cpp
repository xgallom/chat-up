//
// Created by xgallom on 1/23/19.
//

#include "Socket/SocketAddress.h"
#include <ostream>

sockaddr_in SocketAddress::address() const noexcept
{
	return m_address;
}

std::ostream &operator<<(std::ostream &os, const SocketAddress &socketAddress)
{
	const auto address = socketAddress.address();
	return os << IpAddress(address.sin_addr.s_addr) << ":" << address.sin_port;
}
