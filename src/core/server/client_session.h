//
// Created by CGR on 13/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_CLIENT_SESSION_H__
#define __OPEN_CAROM3D_SERVER_CLIENT_SESSION_H__

#include <nettools/ntConnection.h>
#include <core/util/types.h>
#include <vector>

namespace core {

    class Server;

    class ClientSession {
    protected:
        Server& m_server;
        nettools::ntConnection& m_ntClient;
        u32 m_sessionId;
        std::vector<u8> m_pendingDataToRead;
        std::vector<u8> m_pendingDataToSend;

    public:
        ClientSession(nettools::ntConnection& ntClient, Server& server);

        void appendReceivedData(unsigned char data[], unsigned int dataLen) {
            m_pendingDataToRead.insert(m_pendingDataToRead.end(), data, data + dataLen);
        }

        void appendDataToSend(unsigned char data[], unsigned int dataLen) {
            m_pendingDataToRead.insert(m_pendingDataToRead.end(), data, data + dataLen);
        }

        void discardReadPendingData(unsigned int dataLen) {
            m_pendingDataToRead.erase(m_pendingDataToRead.begin(), m_pendingDataToRead.begin() + dataLen);
        }

        void discardSendPendingData(unsigned int dataLen) {
            m_pendingDataToSend.erase(m_pendingDataToSend.begin(), m_pendingDataToSend.begin() + dataLen);
        }

        u8* pendingReadData() {
            return &m_pendingDataToRead[0];
        }

        u8* pendingSendData() {
            return &m_pendingDataToSend[0];
        }

        u32 pendingReadDataSize() const {
            return m_pendingDataToRead.size();
        }

        u32 pendingSendDataSize() const {
            return m_pendingDataToSend.size();
        }

        Server& server() const { return m_server; }
        nettools::ntConnection& ntClient() const { return m_ntClient; }
        u32 sessionId() const { return m_sessionId; }
    };

}

#endif //__OPEN_CAROM3D_SERVER_CLIENTSESSION_H__
