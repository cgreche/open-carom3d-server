#pragma once

#include "ntConnection.h"
#include <string>

namespace nettools {

    class ntEventHandler;

	class ntClient : public ntConnection {
		ntEventHandler* m_eventHandler;

	public:
		ntClient();
		ntClient(ntEventHandler *eventHandler);
		NT_ERROR connect(const char *hostname, u16 port);
		NT_ERROR send(std::string message);
		NT_ERROR send(const unsigned char *data, unsigned int dataLen);
		virtual NT_ERROR poll();
		virtual NT_ERROR close();

		void setEventHandler(ntEventHandler* eventHandler);
		ntEventHandler* eventHandler() const;
	};

}