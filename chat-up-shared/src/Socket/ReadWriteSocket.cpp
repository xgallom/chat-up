//
// Created by xgallom on 1/25/19.
//

#include "Socket/ReadWriteSocket.h"

extern "C" {
#include <sys/socket.h>
}

#include <cstring>

ReadWriteSocket::ReadWriteSocket(int socket) noexcept : Socket(socket) {}

int ReadWriteSocket::readByte() noexcept
{
	char byteRead = 'd';

	const auto readResult = recv(fd(), &byteRead, sizeof(byteRead), MSG_DONTWAIT);

	if(readResult < 0)
		return -1;
	else
		return byteRead;
}

void ReadWriteSocket::readWalk(char *&buf, size_t &remaining) noexcept
{
	const auto readResult = recv(fd(), buf, remaining, MSG_DONTWAIT);

	if(readResult >= 0) {
		const auto bytesRead = static_cast<size_t>(readResult);

		buf += bytesRead;
		remaining -= bytesRead;
	}
}

void ReadWriteSocket::writeByte(char data) noexcept
{
	write(&data, sizeof(char));
}

void ReadWriteSocket::write(const char data[]) noexcept
{
	write(data, strlen(data));
}

void ReadWriteSocket::write(const char *data, size_t length) noexcept
{
	while(length) {
		const auto sendResult = send(fd(), data, length, 0);

		if(sendResult < 0) {
			close();
			return;
		}

		const auto bytesSent = static_cast<size_t>(sendResult);

		length -= bytesSent;
		data += bytesSent;
	}
}

void ReadWriteSocket::write(const std::string &data) noexcept
{
	write(data.c_str(), data.length());
}
