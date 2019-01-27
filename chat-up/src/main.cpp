//
// Created by xgallom on 1/24/19.
//

#include "ClientSocket.h"
#include "ClientSocketConfig.h"
#include "ClientService.h"

#include <iostream>

#include <Messaging/MessageSender.h>
#include <Messaging/Messages/Handshake.h>
#include <Messaging/Version.h>
#include <Socket/SocketException.h>

int main(int argc, char *argv[])
{
	try {
		ConfigManager configManager("config.cfg", argc, argv);
		ClientSocketConfig socketConfig(configManager);

		ClientSocket socket;

		socket.connect(socketConfig.address(IpAddress::LocalHost(), SocketAddress::DefaultPort()));

		ClientService service(socket);

		bool isRunning = true;
		while(socket.isOpen() && isRunning)
			isRunning = service.run();

		return EXIT_SUCCESS;
	} catch(SocketException &e) {
		std::cerr << e.what() << std::endl;
	}

	return EXIT_FAILURE;
}

