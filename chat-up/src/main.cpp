//
// Created by xgallom on 1/24/19.
//

#include "ClientSocket.h"
#include "ClientSocketConfig.h"
#include <Socket/SocketException.h>

#include <netinet/in.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[])
{
	try {
		ConfigManager configManager("server.cfg", argc, argv);
		ClientSocketConfig socketConfig(configManager);

		ClientSocket socket;

		socket.connect(socketConfig.address(IpAddress::LocalHost(), SocketAddress::DefaultPort()));

		return EXIT_SUCCESS;
	} catch(SocketException &e) {
		std::cerr << e.what() << std::endl;
	}

	return EXIT_FAILURE;
}

