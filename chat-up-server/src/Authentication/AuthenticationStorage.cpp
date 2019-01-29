//
// Created by xgallom on 1/27/19.
//

#include "AuthenticationStorage.h"
#include <fstream>
#include <algorithm>
#include <iostream>

static const char storageFileName[] = "authentication.dat";
static const User DefaultUser = {
        {"xgallom"},
        {"Transmeta2"}
};

AuthenticationStorage::AuthenticationStorage() noexcept
{
    std::fstream file(storageFileName, std::ios_base::in);

    if(!file.is_open()) {
        std::cout << "Failed to open storage file " << storageFileName << std::endl;
        return;
    }

    size_t size;
    file.read(reinterpret_cast<char *>(&size), sizeof(size));

    m_users.resize(size);

    file.read(reinterpret_cast<char *>(&*m_users.begin()), static_cast<std::streamsize>(sizeof(User) * size));

    file.close();
}

bool AuthenticationStorage::insert(const User &user) noexcept
{
    std::unique_lock lock(m_accessMutex);

    auto i = std::lower_bound(m_users.cbegin(), m_users.cend(), user, [](const User &l, const User &r) {
        return l.username < r.username;
    });

    if(i == m_users.cend() || *i != user) {
        m_users.insert(i, user);

        update();

        return true;
    }
    else
        return false;
}

bool AuthenticationStorage::remove(const User &user, bool byName) noexcept
{
    std::unique_lock lock(m_accessMutex);

    auto i = std::find_if(m_users.begin(), m_users.end(), [byName, &user](const User &u) {
        return byName ? user.username == u.username : user == u;
    });

    if(i == m_users.end())
        return false;

    m_users.erase(i);
    update();

    return true;
}

bool AuthenticationStorage::contains(const User &user) const noexcept
{
    std::shared_lock lock(m_accessMutex);

    return std::binary_search(m_users.cbegin(), m_users.cend(), user) || user == DefaultUser;
}

void AuthenticationStorage::list(std::ostream &os) const noexcept
{
    std::shared_lock lock(m_accessMutex);

    os << "{\n";

    for(const auto &i : m_users) {
        os << "  {" << i.username << ", " << i.password << "}\n";
    }

    os << "}\n";
}

void AuthenticationStorage::list(std::ostream &os, User user) const noexcept
{
    std::shared_lock lock(m_accessMutex);

    auto begin = std::lower_bound(m_users.begin(), m_users.end(), user, [](const User &l, const User &r) {
        return l.username < r.username;
    });

    {
        auto i = user.username.begin() + 1;
        for(; i != user.username.end() && *i != '\0'; ++i) {}

        (*--i)++;
    }

    auto end = std::upper_bound(begin, m_users.end(), user, [](const User &l, const User &r) {
        return l.username < r.username;
    });

    os << "{\n";

    for(auto i = begin; i != end; ++i) {
        os << "  {" << i->username << ", " << i->password << "}\n";
    }

    os << "}\n";
}

void AuthenticationStorage::update() noexcept
{
    std::fstream file(storageFileName, std::ios_base::out);

    if(!file.is_open()) {
        std::cerr << "Error opening storage file " << storageFileName << std::endl;
        return;
    }


    {
        const auto size = m_users.size();
        file.write(reinterpret_cast<const char *>(&size), sizeof(size));
    }

    file.write(reinterpret_cast<const char *>(&*m_users.cbegin()),
               static_cast<std::streamsize>(sizeof(User) * m_users.size()));

    file.close();
}


AuthenticationStorage &AuthenticationStorage::Instance() noexcept
{
    static AuthenticationStorage instance = AuthenticationStorage();

    return instance;
}
