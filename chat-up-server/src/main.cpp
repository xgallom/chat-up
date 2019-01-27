#include "ServerSocket.h"
#include "ServerSocketConfig.h"
#include "ClientService.h"

#include <Socket/SocketException.h>

#include <iostream>
#include <cstdlib>
#include <memory>
#include <atomic>
#include <thread>
#include <exception>
#include <mutex>
#include <algorithm>

static ServerSocket initialize(int argc, char *argv[]);
static void run(ServerSocket &server);
static void runServer(ServerSocket &server, std::atomic_bool &isRunning);
static void runClientHandler(ClientSocket &&socket, std::atomic_bool &isRunning,
                             std::vector<std::thread> &clientThreads, std::mutex &clientThreadsMutex);

int main(int argc, char *argv[])
{
    try {
        ServerSocket server = initialize(argc, argv);

        run(server);

        return EXIT_SUCCESS;
    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;

        return EXIT_FAILURE;
    }
}

static ServerSocket initialize(int argc, char *argv[])
{
    ConfigManager configManager("server.cfg", argc, argv);
    ServerSocketConfig serverConfig(configManager);

    ServerSocket server;

    server.bind(serverConfig.address(IpAddress::Any(), SocketAddress::DefaultPort()));

    server.listen(serverConfig.maxPendingConnections(4));

    server.maxConcurrentConnections = serverConfig.maxConcurrentConnections(4);
    std::cout << "Server maximum concurrent connections: " << server.maxConcurrentConnections << std::endl;

    std::cout << std::endl;

    return server;
}

static void run(ServerSocket &server)
{
    std::atomic_bool isRunning(true);

    auto serverThread = std::thread(runServer,
                                    std::ref(server),
                                    std::ref(isRunning)
    );

    std::cin.get();

    isRunning = false;
    serverThread.join();
}

static void runServer(ServerSocket &server, std::atomic_bool &isRunning)
{
    std::vector<std::thread> clientThreads;
    std::mutex clientThreadsMutex;

    while(isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        {
            std::lock_guard lockGuard(clientThreadsMutex);

            if(static_cast<int>(clientThreads.size()) >= server.maxConcurrentConnections)
                continue;
        }

        auto clientSocket = server.accept();

        if(clientSocket) {
            std::lock_guard lockGuard(clientThreadsMutex);

            clientThreads.emplace_back(runClientHandler,
                                       std::move(*clientSocket),
                                       std::ref(isRunning),
                                       std::ref(clientThreads),
                                       std::ref(clientThreadsMutex)
            );
        }
    }

    for(;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        std::lock_guard lockGuard(clientThreadsMutex);

        if(clientThreads.empty())
            return;
    }
}

static void
runClientHandler(ClientSocket &&socket, std::atomic_bool &isRunning, std::vector<std::thread> &clientThreads,
                 std::mutex &clientThreadsMutex)
{
    ClientService service(socket);

    while(socket.isOpen() && isRunning) {
        try {
            if(!service.run())
                break;
        } catch(std::exception &e) {
            std::cerr << "Exception from client handler: " << e.what() << std::endl;

            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::lock_guard lockGuard(clientThreadsMutex);

    auto rend = std::remove_if(clientThreads.begin(), clientThreads.end(),
                               [id = std::this_thread::get_id()](std::thread &thread) {
                                   if(thread.get_id() == id) {
                                       thread.detach();
                                       return true;
                                   }

                                   return false;
                               }
    );

    clientThreads.erase(rend, clientThreads.end());
}
