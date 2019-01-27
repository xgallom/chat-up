//
// Created by xgallom on 1/23/19.
//

#include "Socket/IpAddress.h"
#include <sstream>

IpAddress::IpAddress(const std::string &addressString) noexcept
{
    if(inet_pton(AF_INET, addressString.c_str(), &address) != 1)
        *this = Invalid();
}

bool IpAddress::isValid() const noexcept
{
    return *this == Invalid();
}

std::ostream &operator<<(std::ostream &os, const IpAddress ipAddress) noexcept
{
    if(ipAddress == IpAddress::Any())
        return os << "any";
    else if(ipAddress == IpAddress::Invalid())
        return os << "invalid";
    else {
        char buf[INET_ADDRSTRLEN];

        inet_ntop(AF_INET, &ipAddress.address, buf, INET_ADDRSTRLEN);

        return os << buf;
    }
}

std::stringstream &&operator>>(std::stringstream &&ss, IpAddress &ipAddress) noexcept
{
    ipAddress = IpAddress(ss.str());
    return std::move(ss);
}
