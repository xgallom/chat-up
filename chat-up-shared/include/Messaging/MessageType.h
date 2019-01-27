//
// Created by xgallom on 1/26/19.
//

#ifndef CHAT_UP_MESSAGETYPE_H
#define CHAT_UP_MESSAGETYPE_H

namespace MessageType
{
	enum Enum {
		Invalid = 0,

		Handshake,
		HandshakeSuccessful,
		HandshakeFailed,

		Authentication,
		AuthenticationSuccessful,
		AuthenticationFailed,

		Size
	};
}

#endif //CHAT_UP_MESSAGETYPE_H
