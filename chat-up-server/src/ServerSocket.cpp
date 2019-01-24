//
// Created by xgallom on 1/24/19.
//

#include "ServerSocket.h"
#include <Socket/SocketException.h>

extern "C" {
#include <sys/socket.h>
#include <unistd.h>
}

#include <iostream>

void ServerSocket::bind(const SocketAddress &socketAddress)
{
	std::cout << "Binding server socket to address " << socketAddress << std::endl;

	const auto address = socketAddress.address();

	if(::bind(fd(), reinterpret_cast<const sockaddr *>(&address), sizeof(address)))
		throw SocketException("Socket binding failed");
}

void ServerSocket::listen(int maxPending)
{
	std::cout << "Server socket listening to maximum " << maxPending << " connections\n";

	if(::listen(fd(), maxPending))
		throw SocketException("Listen failed");
}

ClientSocket ServerSocket::accept()
{
	int clientSocket = ::accept(fd(), nullptr, nullptr);

	if(!IsValid(clientSocket))
		throw SocketException("Server socked failed accept");

	return ClientSocket(clientSocket);
}
