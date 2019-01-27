//
// Created by xgallom on 1/26/19.
//

#include <Messaging/Messages/Handshake.h>
#include <Messaging/Messages/Authentication.h>
#include <Messaging/Version.h>
#include "ClientService.h"
#include <iostream>

ClientService::ClientService(ClientSocket &socket) noexcept : m_receiver(socket), m_sender(socket) {}

bool ClientService::run()
{
	switch(m_state) {
		case StateHandshaking:
			return runHandshaking();

		case StateHandshakingWaiting:
			return runHandshakingWaiting();

		case StateAuthenticating:
			return runAuthenticating();

		case StateAuthenticatingWaiting:
			return runAuthenticatingWaiting();

		case StateRunning:
			return runRunning();
	}

	std::cerr << "Client service has invalid state\n";
	return false;
}

bool ClientService::runHandshaking()
{
	auto body = createBody<HandshakeMessageBody>();

	body->version.major = MessagingVersion::Major;
	body->version.minor = MessagingVersion::Minor;

	std::cout << "Handshaking with " << static_cast<int>(body->version.major) << "."
			  << static_cast<int>(body->version.minor) << std::endl;

	m_sender.sendMessage<HandshakeMessageBody>(Message(std::move(body)));

	m_state = StateHandshakingWaiting;

	return true;
}

bool ClientService::runHandshakingWaiting()
{
	auto message = m_receiver.receiveMessage();

	if(message.isValid()) {
		switch(message.type()) {
			case MessageType::HandshakeSuccessful:
				std::cout << "Handshake successful\n";

				m_state = StateAuthenticating;
				return true;

			case MessageType::HandshakeFailed: {
				const auto &body = message.bodyAs<HandshakeFailedMessageBody>();

				std::cout << "Handshake failed: Server runs version " << static_cast<int>(body.version.major) << "."
						  << static_cast<int>(body.version.minor) << std::endl;

				return false;
			}

			default:
				std::cout << "Invalid handshake response: " << message.type() << std::endl;
				return false;
		}
	}

	return true;
}

bool ClientService::runAuthenticating()
{
	auto body = createBody<AuthenticationMessageBody>();

	std::cout << "Username: ";
	std::cin.getline(body->username, 16);

	std::cout << "Password: ";
	std::cin.getline(body->password, 16);

	m_sender.sendMessage<AuthenticationMessageBody>(Message(std::move(body)));

	m_state = StateAuthenticatingWaiting;

	return true;
}

bool ClientService::runAuthenticatingWaiting()
{
	return true;
}

bool ClientService::runRunning()
{
	return true;
}
