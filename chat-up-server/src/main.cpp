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
#include <Authentication/AuthenticationStorage.h>

static ServerSocket initialize(int argc, char *argv[]);
static void run(ServerSocket &server);
static void runServer(ServerSocket &server, std::atomic_bool &isRunning);
static void runClientHandler(std::unique_ptr<ClientSocket> &&socket, std::atomic_bool &isRunning,
                             std::vector<std::thread> &clientThreads, std::vector<ClientService *> &clientServices,
                             std::mutex &clientThreadsMutex);

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

    std::string input;
    while(input != "exit") {
        std::cout << "$ ";
        std::getline(std::cin, input);

        if(input == "auth" || input == "auth list") {
            std::cout << "auth list ";
            AuthenticationStorage::Instance().list(std::cout);
        }
        else if(input == "auth find") {
            User user;

            std::cout << "username: ";
            std::getline(std::cin, input);
            copy(user.username,  input);

            AuthenticationStorage::Instance().list(std::cout, user);
        }
        else if(input == "auth remove") {
            User user;

            std::cout << "username: ";
            std::getline(std::cin, input);
            copy(user.username,  input);

            AuthenticationStorage::Instance().remove(user, true);
        }
        else if(input == "register") {
            User user;

            std::cout << "username: ";
            std::getline(std::cin, input);
            copy(user.username, input);

            std::cout << "password: ";
            std::getline(std::cin, input);
            copy(user.password, input);

            AuthenticationStorage::Instance().insert(user);
        }
    }

    isRunning = false;
    serverThread.join();
}

static void runServer(ServerSocket &server, std::atomic_bool &isRunning)
{
    std::vector<std::thread> clientThreads;
    std::vector<ClientService *> clientServices;
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
                                       std::move(clientSocket),
                                       std::ref(isRunning),
                                       std::ref(clientThreads),
                                       std::ref(clientServices),
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
runClientHandler(std::unique_ptr<ClientSocket> &&socket, std::atomic_bool &isRunning, std::vector<std::thread> &clientThreads,
                 std::vector<ClientService *> &clientServices, std::mutex &clientThreadsMutex)
{
    ClientService service(*socket, {std::ref(clientServices), std::ref(clientThreadsMutex)});

    {
        std::lock_guard lockGuard(clientThreadsMutex);
        clientServices.push_back(&service);
    }

    while(socket->isOpen() && isRunning) {
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

    {
        const auto rend = std::remove_if(clientThreads.begin(), clientThreads.end(),
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

    {
        const auto rend = std::remove_if(clientServices.begin(), clientServices.end(),
                                         [&service](ClientService *clientService) {
                                             return &service == clientService;
                                         });

        clientServices.erase(rend, clientServices.end());
    }
}
