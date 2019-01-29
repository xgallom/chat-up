//
// Created by xgallom on 1/24/19.
//

#include "ClientSocket.h"
#include "Ui.h"
#include <Socket/SocketException.h>
#include <csignal>

extern "C" {
#include <sys/socket.h>
#include <unistd.h>
}

ClientSocket::ClientSocket()
{
    signal(SIGPIPE, SIG_IGN);
}

void ClientSocket::connect(const SocketAddress &socketAddress)
{
    std::stringstream ss;
    ss << "Connecting to " << socketAddress;

    auto messageWindow = Ui::Window::MessagePrompt(ss.str().c_str(), "Connecting", Ui::MessageInfo);
    messageWindow.refresh();

    const auto address = socketAddress.address();

    if(::connect(fd(), reinterpret_cast<const sockaddr *>(&address), sizeof(address))) {
        Ui::MessagePrompt("Couldn't connect to server.", "Error", Ui::MessageError);

        throw SocketException("Socket connection failed");
    }
}
