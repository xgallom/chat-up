//
// Created by xgallom on 1/26/19.
//

#include <Messaging/Messages/Handshake.h>
#include <Messaging/Version.h>
#include <Messaging/MessageException.h>
#include <Messaging/Messages/Authentication.h>
#include <iostream>
#include "ClientService.h"

ClientService::ClientService(ClientSocket &socket) noexcept : m_receiver(socket), m_sender(socket) {}

void ClientService::run()
{
	const auto message = m_receiver.receiveMessage();

	if(!message.isValid())
		return;

	switch(m_state) {
		case StateHandshaking:
			runHandshaking(message);
			break;

		case StateAuthenticating:
			runAuthenticating(message);
			break;

		case StateRunning:
			runRunning(message);
			break;
	}
}

void ClientService::runHandshaking(const Message &message)
{
	if(message.type() == MessageType::Handshake) {
		const auto &body = message.bodyAs<HandshakeMessageBody>();

		if(body.version.major != MessagingVersion::Major ||
		   body.version.minor != MessagingVersion::Minor) {
			auto responseBody = createBody<HandshakeFailedMessageBody>();
			responseBody->version.major = MessagingVersion::Major;
			responseBody->version.minor = MessagingVersion::Minor;

			m_sender.sendMessage<HandshakeFailedMessageBody>(Message(std::move(responseBody)));

			throw MessageException("Invalid messaging version");
		}

		m_sender.sendMessage<HandshakeSuccessfulMessageBody>(Message::Create<HandshakeSuccessfulMessageBody>());

		m_state = StateAuthenticating;
	}
}

void ClientService::runAuthenticating(const Message &message)
{
	if(message.type() == MessageType::Authentication) {
		const auto &body = message.bodyAs<AuthenticationMessageBody>();

		std::cout << "Authentication with\n  Username: " << body.username << "\n  Password: " << body.password
				  << std::endl;
	}
}

void ClientService::runRunning(const Message &message)
{

}
