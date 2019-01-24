//
// Created by xgallom on 1/24/19.
//

#ifndef CHAT_UP_SERVERSOCKETCONFIG_H
#define CHAT_UP_SERVERSOCKETCONFIG_H

#include <Socket/SocketConfig.h>

class ServerSocketConfig : public SocketConfig {
public:
	ServerSocketConfig(const ConfigManager &configManager) noexcept;

	int maxPendingConnections(int defaultValue) noexcept;
};

#endif //CHAT_UP_SERVERSOCKETCONFIG_H
