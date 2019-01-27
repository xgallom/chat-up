//
// Created by xgallom on 1/25/19.
//

#include "Messaging/MessageReceiver.h"
#include "Socket/ReadWriteSocket.h"
#include "Messaging/MessageOpener.h"

MessageReceiver::MessageReceiver(ReadWriteSocket &socket) noexcept :
		m_socket(socket) {}

Message MessageReceiver::receiveMessage()
{
	switch(m_state) {
		case ReadingOpener:
			return readOpener();

		case ReadingHeader:
			return readHeader();

		case ReadingBody:
			return readBody();
	}

	return Message();
}

Message MessageReceiver::readOpener()
{
	const auto readResult = m_socket.readByte();

	if(readResult != MessageOpener)
		return Message();

	m_state = ReadingHeader;
	m_remaining = sizeof(MessageHeader);

	return readHeader();
}

Message MessageReceiver::readHeader()
{
	auto buf = &reinterpret_cast<char *>(&m_currentMessage.header)[sizeof(MessageHeader) - m_remaining];

	m_socket.readWalk(buf, m_remaining);

	if(m_remaining)
		return Message();

	auto body = createBody(m_currentMessage.header.type);

	m_state = ReadingBody;
	m_currentMessage.body = std::move(body);
	m_remaining = m_currentMessage.header.length;

	return readBody();
}

Message MessageReceiver::readBody() noexcept
{
	auto buf = &reinterpret_cast<char *>(m_currentMessage.body.get())[m_currentMessage.header.length - m_remaining];

	m_socket.readWalk(buf, m_remaining);

	if(m_remaining)
		return Message();

	m_state = ReadingOpener;
	m_remaining = 1;

	return std::move(m_currentMessage);
}
