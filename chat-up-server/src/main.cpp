#include "ServerSocket.h"
#include "ServerSocketConfig.h"
#include <Socket/SocketException.h>

#include <netinet/in.h>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <atomic>
#include <thread>

static ServerSocket initialize(int argc, char *argv[])
{
	ConfigManager configManager("server.cfg", argc, argv);
	ServerSocketConfig socketConfig(configManager);

	ServerSocket server;

	server.bind(socketConfig.address(IpAddress::Any(), SocketAddress::DefaultPort()));

	server.listen(socketConfig.maxPendingConnections(4));

	return server;
}

static void runClientHandler(ClientSocket &&socket)
{

}

static void runServer(ServerSocket &server, std::atomic_bool &isRunning, std::vector<std::thread> &clientThreads)
{
	while(isRunning) {
		ClientSocket clientSocket = server.accept();

		clientThreads.emplace_back(runClientHandler, std::move(clientSocket));
	}
}

static void run(ServerSocket &server)
{
	std::atomic_bool isRunning = true;
	std::vector<std::thread> clientThreads;

	std::thread serverThread = std::thread(runServer, std::ref(server), std::ref(isRunning), std::ref(clientThreads));

	std::cin.get();

	isRunning = false;
	serverThread.join();

	for(auto &thread : clientThreads)
		thread.join();
}

int main(int argc, char *argv[])
{
	try {
		ServerSocket server = initialize(argc, argv);

		run(server);

		return EXIT_SUCCESS;
	} catch(SocketException &e) {
		std::cerr << e.what() << std::endl;
	}

	return EXIT_FAILURE;
}
