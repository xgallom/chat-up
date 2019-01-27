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

	if(containsUnsafe(user))
		return false;

	m_users.push_back(user);
	std::sort(m_users.begin(), m_users.end());

	update();

	return true;
}

bool AuthenticationStorage::remove(const User &user) noexcept
{
	std::unique_lock lock(m_accessMutex);

	auto i = std::find(m_users.begin(), m_users.end(), user);

	if(i == m_users.end())
		return false;

	m_users.erase(i);
	update();

	return true;
}

bool AuthenticationStorage::contains(const User &user) noexcept
{
	std::shared_lock lock(m_accessMutex);

	return containsUnsafe(user) || user == DefaultUser;
}

bool AuthenticationStorage::containsUnsafe(const User &user) noexcept
{
	return std::binary_search(m_users.cbegin(), m_users.cend(), user);
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
