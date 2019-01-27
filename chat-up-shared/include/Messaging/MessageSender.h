//
// Created by xgallom on 1/25/19.
//

#ifndef CHAT_UP_MESSAGESENDER_H
#define CHAT_UP_MESSAGESENDER_H

#include "Message.h"
#include "MessageOpener.h"

class ReadWriteSocket;

class MessageSender {
	ReadWriteSocket &m_socket;

public:
	explicit MessageSender(ReadWriteSocket &socket) noexcept;

	template<typename BodyType>
	void sendMessage(const Message &message) const noexcept
	{
		m_socket.writeByte(MessageOpener);

		const char *buf = reinterpret_cast<const char *>(&message.header);

		m_socket.write(buf, sizeof(MessageHeader));

		writeBody(m_socket, static_cast<const BodyType &>(*message.body));
	}
};

#endif //CHAT_UP_MESSAGESENDER_H
