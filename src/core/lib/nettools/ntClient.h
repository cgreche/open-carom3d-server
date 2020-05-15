#pragma once

#include "ntConnection.h"
#include <string>

namespace nettools {

	class ntClient : public ntConnection {
	public:
		ntClient();
		NT_ERROR connect(const char *hostname, u16 port);
		NT_ERROR send(std::string message);
		NT_ERROR send(const unsigned char *data, unsigned int dataLen);
        //TODO: std::string only is of no use, add const u8 data and std::vector<u8> data
		virtual NT_ERROR poll();
		virtual NT_ERROR close();
	};

}