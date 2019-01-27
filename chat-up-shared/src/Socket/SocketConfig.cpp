//
// Created by xgallom on 1/24/19.
//

#include "Socket/SocketConfig.h"

SocketConfig::SocketConfig(const ConfigManager &configManager) noexcept : m_configManager(configManager) {}

SocketAddress SocketConfig::address(IpAddress defaultIp, uint16_t defaultPort) noexcept
{
    const auto
            ipCfg = m_configManager.find("ip"),
            portCfg = m_configManager.find("port");

    IpAddress ip;
    if(m_configManager.exists(ipCfg))
        ipCfg.value() >> ip;
    else
        ip = defaultIp;

    uint16_t port;
    if(m_configManager.exists(portCfg))
        portCfg.value() >> port;
    else
        port = defaultPort;

    return SocketAddress(ip, port);
}

