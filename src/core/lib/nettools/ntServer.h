#pragma once

#include "ntConnection.h"
#include <list>

namespace nettools {

	class ntServer : public ntConnection
	{
		std::list<ntConnection*> m_clients;

	public:
		ntServer();
		NT_ERROR listen(u16 port);
		virtual NT_ERROR poll();
        virtual NT_ERROR disconnect(ntConnection *client);
		virtual NT_ERROR close();

        unsigned int connectionCount() const;
	};

}