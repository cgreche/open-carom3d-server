
#include "ntServer.h"
#include "ntEventHandler.h"

namespace nettools {
	
	bool ntServer::checkPendingDisconnection(ntConnection* connection) {
		std::set<ntConnection*>::iterator it = m_pendingDisconnections.find(connection);
		if(it != m_pendingDisconnections.end()) {
			m_pendingDisconnections.erase(it);
			return true;
		}
		return false;
	}

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
                    m_eventHandler->onClientConnection(newClient);
			}
		} while (result == NTERR_SUCCESS);

		std::list<ntConnection*>::iterator it = m_clients.begin();
		for(; it != m_clients.end(); ) {
		    ntConnection* conn = *it;
			u8 data[MAX_PACKET_SIZE];
			u32 recvd;

			bool disconnectClient = false;

			if(checkPendingDisconnection(conn)) {
				disconnectClient = true;
				conn->close();
			}
			else {
				result = _recv(conn->socket(), data, MAX_PACKET_SIZE, &recvd);
				switch(result) {
					case NTERR_SUCCESS: {
						if(recvd == 0) {
							disconnectClient = true;
						}
						else {
							if(m_eventHandler) {
								//TODO(CGR): m_Clients must be of ntClient type
								m_eventHandler->onClientDataReceived(conn, data, recvd);
							}
						}

						break;
					}

					case NTERR_OTHER: {
						disconnectClient = true;
						break;
					}
				}
			}

			if(disconnectClient) {
				m_eventHandler->onClientDisconnection(*it);
				it = m_clients.erase(it);
				continue;
			}

            ++it;
		}
		return NTERR_SUCCESS;
	}

    NT_ERROR ntServer::disconnect(ntConnection *connection) {
		m_pendingDisconnections.insert(connection);
        return NTERR_SUCCESS;
    }

	NT_ERROR ntServer::close() {
		ntConnection::close();
		return NTERR_SUCCESS;
	}

    unsigned int ntServer::connectionCount() const {
        return m_clients.size();
    }

	void ntServer::setEventHandler(ntEventHandler* eventHandler) {
		m_eventHandler = eventHandler;
	}

	ntEventHandler* ntServer::eventHandler() const {
		return m_eventHandler;
	}
}