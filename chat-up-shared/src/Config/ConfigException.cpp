//
// Created by xgallom on 1/23/19.
//

#include "Config/ConfigException.h"

ConfigException::ConfigException(const char msg[]) noexcept : std::logic_error(msg) {}
