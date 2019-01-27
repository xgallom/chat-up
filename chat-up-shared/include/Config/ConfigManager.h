//
// Created by xgallom on 1/23/19.
//

#ifndef CHAT_UP_CONFIGMANAGER_H
#define CHAT_UP_CONFIGMANAGER_H

#include "Config.h"
#include <vector>

class ConfigManager {
private:
	 Config::MapType m_config = Config::MapType();

public:
	explicit ConfigManager(const char fileName[], int argc, char *argv[]) noexcept;

	ConfigManager(const ConfigManager &) = default;
	ConfigManager &operator=(const ConfigManager &) = default;

	ConfigManager(ConfigManager &&) noexcept = default;
	ConfigManager &operator=(ConfigManager &&) noexcept = default;

	Config find(const std::string &key) const noexcept;
	bool exists(const Config &config) const noexcept;

private:
	void parse(int argc, char *argv[]) noexcept;
	void parse(std::fstream &file) noexcept;
	void parse(const std::string &line) noexcept;
};

#endif //CHAT_UP_CONFIGMANAGER_H
