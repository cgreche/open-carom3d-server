#pragma once

#include "ntConnection.h"
#include <vector>

namespace nettools {

	class ntServer : public ntConnection
	{
		std::vector<ntConnection*> m_clients;
	public:
		ntServer();
		NT_ERROR listen(u16 port);
		virtual NT_ERROR poll();
		virtual NT_ERROR close();
	};

}