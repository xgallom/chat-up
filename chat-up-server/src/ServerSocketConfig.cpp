//
// Created by xgallom on 1/24/19.
//

#include "ServerSocketConfig.h"

ServerSocketConfig::ServerSocketConfig(const ConfigManager &configManager) noexcept : SocketConfig(configManager) {}

int ServerSocketConfig::maxPendingConnections(int defaultValue) noexcept
{
    const auto valueCfg = m_configManager.find("maxPending");

    int value;

    if(m_configManager.exists(valueCfg))
        valueCfg.value() >> value;
    else
        value = defaultValue;

    return value;
}

int ServerSocketConfig::maxConcurrentConnections(int defaultValue) noexcept
{
    const auto valueCfg = m_configManager.find("maxConcurrent");

    int value;

    if(m_configManager.exists(valueCfg))
        valueCfg.value() >> value;
    else
        value = defaultValue;

    return value;
}
