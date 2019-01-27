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

	if(readResult > 0)
		return byteRead;
	else if(!readResult || errno != EAGAIN)
		close();

	return -1;
}

void ReadWriteSocket::readWalk(char *&buf, size_t &remaining) noexcept
{
	const auto readResult = recv(fd(), buf, remaining, MSG_DONTWAIT);

	if(readResult > 0) {
		const auto bytesRead = static_cast<size_t>(readResult);

		buf += bytesRead;
		remaining -= bytesRead;
	}
	else if(!readResult || errno != EAGAIN)
		close();
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

		if(sendResult >= 0) {
			const auto bytesSent = static_cast<size_t>(sendResult);

			length -= bytesSent;
			data += bytesSent;
		}
		else
			close();
	}
}

void ReadWriteSocket::write(const std::string &data) noexcept
{
	write(data.c_str(), data.length());
}
