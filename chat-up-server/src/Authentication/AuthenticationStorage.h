//
// Created by xgallom on 1/27/19.
//

#ifndef CHAT_UP_AUTHENTICATIONSTORAGE_H
#define CHAT_UP_AUTHENTICATIONSTORAGE_H

#include "Authentication/User.h"
#include <vector>
#include <shared_mutex>

class AuthenticationStorage {
private:
    std::vector<User> m_users = std::vector<User>();
    std::shared_mutex m_accessMutex = std::shared_mutex();

public:
    AuthenticationStorage() noexcept;

    bool insert(const User &user) noexcept;
    bool remove(const User &user) noexcept;

    bool contains(const User &user) noexcept;

private:
    bool containsUnsafe(const User &user) noexcept;

    void update() noexcept;

public:
    static AuthenticationStorage &Instance() noexcept;
};

#endif //CHAT_UP_AUTHENTICATIONSTORAGE_H
