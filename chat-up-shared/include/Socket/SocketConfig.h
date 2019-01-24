//
// Created by xgallom on 1/23/19.
//

#ifndef CHAT_UP_SOCKETCONFIG_H
#define CHAT_UP_SOCKETCONFIG_H

#include "SocketAddress.h"
#include "Config/ConfigManager.h"

class SocketConfig {
protected:
	const ConfigManager &m_configManager;

public:
	explicit SocketConfig(const ConfigManager &configManager) noexcept;

	SocketAddress address(IpAddress defaultIp, uint16_t defaultPort) noexcept;
};

#endif //CHAT_UP_SOCKETCONFIG_H
