//
// Created by xgallom on 1/23/19.
//

#include "Config/Config.h"

Config::Config(const Config::ConstIterator a_index) noexcept : index{a_index} {}

std::stringstream Config::value() const noexcept
{
    return std::stringstream(index->second);
}
