//
// Created by xgallom on 1/24/19.
//

#ifndef CHAT_UP_CLIENTSOCKETCONFIG_H
#define CHAT_UP_CLIENTSOCKETCONFIG_H

#include <Socket/SocketConfig.h>

class ClientSocketConfig : public SocketConfig {
public:
	ClientSocketConfig(const ConfigManager &configManager) noexcept;
};

#endif //CHAT_UP_CLIENTSOCKETCONFIG_H
