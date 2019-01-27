//
// Created by xgallom on 1/27/19.
//

#ifndef CHAT_UP_USER_H
#define CHAT_UP_USER_H

#include <array>
#include <istream>

struct User {
	std::array<char, 16>
	        username = {},
			password = {};

	bool operator<(const User &r) const noexcept;
	bool operator==(const User &r) const noexcept;
};

template<size_t N>
std::ostream &operator<<(std::ostream &os, const std::array<char, N> &x)
{
	return os << x.cbegin();
}

template<size_t N>
void getline(std::istream &is, std::array<char, N> &x)
{
	is.getline(x.begin(), static_cast<std::streamsize>(x.size()));
}

#endif //CHAT_UP_USER_H
