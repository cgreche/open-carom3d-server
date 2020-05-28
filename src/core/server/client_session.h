//
// Created by CGR on 13/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_CLIENT_SESSION_H__
#define __OPEN_CAROM3D_SERVER_CLIENT_SESSION_H__

#include <vector>

#include <nettools/ntClient.h>
#include "messaging/crypto.h"
#include "action.h"

namespace core {

    class Server;

    class ClientSession {
        Server &m_server;
        int m_sessionId;
        nettools::ntClient &m_ntClient;
        std::vector<char> m_unparsedData;
        CryptoContext m_inDataCryptoCtx;
        CryptoContext m_outDataCryptoCtx;
        std::vector<ActionData> m_pendingActions;

    public:
        ClientSession(nettools::ntClient &ntClient, Server &server);

        void appendUnparsedData(unsigned char data[], unsigned int dataLen) {
            m_unparsedData.insert(m_unparsedData.end(), data, data + dataLen);
        }

        void appendActions(std::vector<ActionData> actionDatas) {
            m_pendingActions.insert(m_pendingActions.end(), actionDatas.begin(), actionDatas.end());
        };

        CryptoContext &inDataCryptoCtx() { return m_inDataCryptoCtx; }

        CryptoContext &outDataCryptoCtx() { return m_outDataCryptoCtx; }

        const std::vector<ActionData> &pendingActions() const { return m_pendingActions; }

        void clearPendingActions() {
            if (!m_pendingActions.empty())
                m_pendingActions.clear();
        }

        void sendAction(const ActionData &action);

        int sessionId() const { return m_sessionId; }
        Server& server() const { return m_server; }
    };

}

#endif //__OPEN_CAROM3D_SERVER_CLIENTSESSION_H__
