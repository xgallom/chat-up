//
// Created by xgallom on 1/24/19.
//

#include "ClientSocket.h"
#include "ClientSocketConfig.h"
#include "ClientService.h"
#include "Ui.h"

#include <iostream>

#include <Messaging/MessageSender.h>
#include <Messaging/Messages/Handshake.h>
#include <Messaging/Version.h>
#include <Socket/SocketException.h>
#include <chrono>
#include <thread>

int main(int argc, char *argv[])
{
    try {
        ConfigManager configManager("config.cfg", argc, argv);
        ClientSocketConfig socketConfig(configManager);

        Ui::Ui ui;

        ClientSocket socket;

        socket.connect(socketConfig.address(IpAddress::LocalHost(), SocketAddress::DefaultPort()));

        ClientService service(socket);

        bool isRunning = true;
        while(socket.isOpen() && isRunning) {
            isRunning = service.run();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        if(isRunning)
            Ui::MessagePrompt("Connection closed.", "Error", Ui::MessageError);

        return EXIT_SUCCESS;
    } catch(SocketException &e) {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_FAILURE;
}

