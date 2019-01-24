//
// Created by xgallom on 1/23/19.
//

#ifndef CHAT_UP_CONFIG_H
#define CHAT_UP_CONFIG_H

#include <unordered_map>
#include <string>
#include <sstream>

class Config {
public:
	using MapType = std::unordered_map<std::string, std::string>;
	using ConstIterator = MapType::const_iterator;

	ConstIterator index;

	explicit Config(ConstIterator a_index) noexcept;

	std::stringstream value() const noexcept;
};

#endif //CHAT_UP_CONFIG_H
