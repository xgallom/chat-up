//
// Created by xgallom on 1/23/19.
//

#ifndef CHAT_UP_IPADDRESS_H
#define CHAT_UP_IPADDRESS_H

extern "C" {
#include <netinet/in.h>
#include <arpa/inet.h>
}

#include <string>

struct IpAddress {
	in_addr_t address = INADDR_NONE;

	constexpr IpAddress() noexcept = default;
	explicit constexpr IpAddress(const in_addr_t a_address) noexcept : address{a_address} {}
	explicit IpAddress(const std::string &addressString) noexcept;

	IpAddress(const IpAddress &) noexcept = default;
	IpAddress(IpAddress &&) noexcept = default;

	IpAddress &operator=(const IpAddress &) noexcept = default;
	IpAddress &operator=(IpAddress &&) noexcept = default;

	bool isValid() const noexcept;

	static constexpr IpAddress Any() noexcept { return IpAddress(INADDR_ANY); }
	static IpAddress LocalHost() noexcept { return IpAddress(0x0100007f); }
	static constexpr IpAddress Invalid() noexcept { return IpAddress(); }

	inline bool operator ==(IpAddress r) const noexcept { return address == r.address; }
};

std::ostream &operator<<(std::ostream &os, IpAddress ipAddress) noexcept;
std::stringstream &&operator>>(std::stringstream &&ss, IpAddress &ipAddress) noexcept;

#endif //CHAT_UP_IPADDRESS_H
