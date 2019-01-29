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
    using Users = std::vector<User>;
    Users m_users = std::vector<User>();
    mutable std::shared_mutex m_accessMutex = std::shared_mutex();

public:
    AuthenticationStorage() noexcept;

    bool insert(const User &user) noexcept;
    bool remove(const User &user, bool byName = false) noexcept;

    bool contains(const User &user) const noexcept;

    void list(std::ostream &os) const noexcept;
    void list(std::ostream &os, User user) const noexcept;

private:
    void update() noexcept;

public:
    static AuthenticationStorage &Instance() noexcept;
};

#endif //CHAT_UP_AUTHENTICATIONSTORAGE_H
