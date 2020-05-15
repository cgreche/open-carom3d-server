
#include "ntServer.h"

namespace nettools {

	ntServer::ntServer() {
	}

	NT_ERROR ntServer::listen(u16 port) {
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.S_un.S_addr = ADDR_ANY;
		NT_ERROR result;
		result = _bind(m_socket, (sockaddr*)&addr, sizeof(addr));
		if (result != NTERR_SUCCESS) {
			_close(m_socket);
			return result;
		}
		return _listen(m_socket);
	}

	NT_ERROR ntServer::poll() {
		unsigned int i;

		NT_ERROR result;
		ntConnection *newClient;
		do {
			result = _accept(m_socket,&newClient);
			if (newClient) {
				m_clients.push_back(newClient);
				if (m_messageHandler)
					m_messageHandler(NT_EVT_CONNECT, newClient, NULL, NULL);
			}
		} while (result == NTERR_SUCCESS);


		for (i = 0; i < m_clients.size(); ++i) {
			u8 data[MAX_PACKET_SIZE];
			u32 recvd;
			result = _recv(m_clients[i]->socket(), data, MAX_PACKET_SIZE, &recvd);
			if (result == NTERR_SUCCESS) {
				if (m_messageHandler) {
					if (recvd == 0)
						m_messageHandler(NT_EVT_DISCONNECT, m_clients[i], NULL, NULL);
					else
						m_messageHandler(NT_EVT_DATARECV, m_clients[i], data, recvd);
				}
			}

		}
		return NTERR_SUCCESS;
	}

	NT_ERROR ntServer::close() {
		ntConnection::close();
		return NTERR_SUCCESS;
	}

}