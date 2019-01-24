//
// Created by xgallom on 1/23/19.
//

#include "Config/ConfigException.h"
#include "Config/ConfigManager.h"

#include <iostream>
#include <fstream>

ConfigManager::ConfigManager(const char fileName[], int argc, char *argv[]) noexcept
{
	std::cout << "Initializing config manager with config file \"" << fileName << "\"\n";

	std::fstream file(fileName, std::ios_base::in);

	if(!file.is_open())
		std::cout << "Failed opening config file\n";
	else {
		parse(file);
		file.close();
	}

	parse(argc, argv);

	std::cout << std::endl;
}

Config ConfigManager::find(const std::string &key) const noexcept
{
	return Config(m_config.find(key));
}

bool ConfigManager::exists(const Config &config) const noexcept
{
	return config.index != m_config.end();
}

void ConfigManager::parse(int argc, char *argv[]) noexcept
{
	for(int n = 1; n < argc; ++n)
		parse(argv[n]);
}

void ConfigManager::parse(std::fstream &file) noexcept
{
	for(std::string line; std::getline(file, line);)
		parse(line);
}

void ConfigManager::parse(const std::string &line) noexcept
{
	if(line.empty())
		return;

	const auto i = line.find('=');

	if(i != std::string::npos)
		m_config[line.substr(0, i)] = line.substr(i + 1);
	else
		std::cout << "  Failed to parse line \"" << line << "\"\n";
}
