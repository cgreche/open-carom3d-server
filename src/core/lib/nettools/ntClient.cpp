
#include "ntClient.h"

namespace nettools {

	ntClient::ntClient() {
	}

	NT_ERROR ntClient::connect(const char * hostname, u16 port) {
		m_address.sin_family = AF_INET;
		::inet_pton(AF_INET, hostname, &m_address.sin_addr);
		m_address.sin_port = ::htons(port);
		return _connect(m_socket, (sockaddr*)&m_address, sizeof(m_address));
	}

	NT_ERROR ntClient::send(std::string message) {
		return ntConnection::send(m_socket, (const u8*)message.c_str(), message.length());
	}

	NT_ERROR ntClient::send(const unsigned char * data, unsigned int dataLen) {
        return ntConnection::send(m_socket, data, dataLen);
	}

	NT_ERROR ntClient::poll() {
		u8 data[MAX_PACKET_SIZE];
		u32 bytesRecv;
		NT_ERROR result = _recv(m_socket, data, MAX_PACKET_SIZE,&bytesRecv);
		if (result != NTERR_SUCCESS) {
			if (bytesRecv == 0) {
				//connection closed
			}
		}
		return result;
	}

	NT_ERROR ntClient::close() {
		ntConnection::close();
		return NTERR_SUCCESS;
	}
}
