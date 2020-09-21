#pragma once

#include "ntConnection.h"
#include <list>
#include <set>

namespace nettools {

	class ntServer : public ntConnection
	{
		std::list<ntConnection*> m_clients;
		std::set<ntConnection*> m_pendingDisconnections;

		ntEventHandler* m_eventHandler;

		bool checkPendingDisconnection(ntConnection* connection);

	public:
		ntServer();
		NT_ERROR listen(u16 port);
		virtual NT_ERROR poll();
        virtual NT_ERROR disconnect(ntConnection *client);
		virtual NT_ERROR close();

        unsigned int connectionCount() const;

		void setEventHandler(ntEventHandler* eventHandler);
		ntEventHandler* eventHandler() const;
	};

}