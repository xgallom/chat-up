//
// Created by xgallom on 1/27/19.
//

#include "Authentication/User.h"
#include <algorithm>

bool User::operator<(const User &r) const noexcept
{
    return username < r.username || password < r.password;
}

bool User::operator==(const User &r) const noexcept
{
    return username == r.username && password == r.password;
}

bool User::operator!=(const User &r) const noexcept
{
    return username != r.username;
}
