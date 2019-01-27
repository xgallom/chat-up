//
// Created by xgallom on 1/26/19.
//

#include <Messaging/Messages/Handshake.h>
#include <Messaging/Messages/Authentication.h>
#include <Messaging/Version.h>
#include "ClientService.h"
#include <iostream>

ClientService::ClientService(ClientSocket &socket) noexcept : m_receiver(socket), m_sender(socket) {}

Outcome::Enum ClientService::run()
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

	return Outcome::Failed;
}

Outcome::Enum ClientService::runHandshaking()
{
	auto body = createBody<HandshakeMessageBody>();

	body->version.major = MessagingVersion::Major;
	body->version.minor = MessagingVersion::Minor;

	std::cout << "Handshaking with " << static_cast<int>(body->version.major) << "."
			  << static_cast<int>(body->version.minor) << std::endl;

	m_sender.sendMessage<HandshakeMessageBody>(Message(std::move(body)));

	m_state = StateHandshakingWaiting;

	return Outcome::Success;
}

Outcome::Enum ClientService::runHandshakingWaiting()
{
	const auto message = m_receiver.receiveMessage();

	if(message.isValid()) {
		switch(message.type()) {
			case MessageType::HandshakeSuccessful:
				std::cout << "Handshake successful\n";

				m_state = StateAuthenticating;

				return Outcome::Success;

			case MessageType::HandshakeFailed: {
				const auto &body = message.bodyAs<HandshakeFailedMessageBody>();

				std::cout << "Handshake failed: Server runs version " << static_cast<int>(body.version.major) << "."
						  << static_cast<int>(body.version.minor) << std::endl;

				return Outcome::Failed;
			}

			default:
				std::cerr << "Invalid handshake response: " << message.type() << std::endl;

				return Outcome::Failed;
		}
	}

	return Outcome::Retry;
}

Outcome::Enum ClientService::runAuthenticating()
{
	auto body = createBody<AuthenticationMessageBody>();

	std::cout << "Username: ";
	getline(std::cin, body->user.username);

	std::cout << "Password: ";
	getline(std::cin, body->user.password);

	m_sender.sendMessage<AuthenticationMessageBody>(Message(std::move(body)));

	m_state = StateAuthenticatingWaiting;

	return Outcome::Success;
}

Outcome::Enum ClientService::runAuthenticatingWaiting()
{
	const auto message = m_receiver.receiveMessage();

	if(message.isValid()) {
		switch(message.type()) {
			case MessageType::AuthenticationSuccessful:
				std::cout << "Authentication successful\n";

				m_state = StateRunning;

				return Outcome::Success;

			case MessageType::AuthenticationFailed:
				std::cout << "Authentication failed\n";

				m_state = StateAuthenticating;

				break;

			default:
				std::cerr << "Invalid handshake response: " << message.type() << std::endl;

				return Outcome::Failed;
		}
	}

	return Outcome::Retry;
}

Outcome::Enum ClientService::runRunning()
{
	return Outcome::Retry;
}
