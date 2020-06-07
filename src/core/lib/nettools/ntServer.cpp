
#include "ntServer.h"
#include "ntEventHandler.h"

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
				if (m_eventHandler)
				    //TODO(CGR): newClient must be of ntClient* type
                    m_eventHandler->onClientConnection((ntClient*)newClient);
			}
		} while (result == NTERR_SUCCESS);

		std::list<ntConnection*>::iterator it = m_clients.begin();
		for(; it != m_clients.end(); ) {
		    ntConnection* conn = *it;
			u8 data[MAX_PACKET_SIZE];
			u32 recvd;
			result = _recv(conn->socket(), data, MAX_PACKET_SIZE, &recvd);
			if (result == NTERR_SUCCESS) {
				if (m_eventHandler) {
                    if(recvd == 0) {
                        m_eventHandler->onClientDisconnection((ntClient*)*it);
                        it = m_clients.erase(it);
                        continue;
                    } else {
                        //TODO(CGR): m_Clients must be of ntClient type
                        m_eventHandler->onClientDataReceived((ntClient *) conn, data, recvd);
                    }
				}
			}

            ++it;
		}
		return NTERR_SUCCESS;
	}

    NT_ERROR ntServer::disconnect(ntConnection *connection) {
        std::list<ntConnection*>::iterator it = std::find(m_clients.begin(), m_clients.end(), connection);
        if(it != m_clients.end()) {
            m_eventHandler->onClientDisconnection((ntClient*)connection);
            m_clients.erase(it);
            return connection->close();
        }
        return NTERR_SUCCESS;
    }

	NT_ERROR ntServer::close() {
		ntConnection::close();
		return NTERR_SUCCESS;
	}

    unsigned int ntServer::connectionCount() const {
        return m_clients.size();
    }

}