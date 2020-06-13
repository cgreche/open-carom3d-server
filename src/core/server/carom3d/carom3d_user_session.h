//
// Created by CGR on 13/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_CAROM3D_USER_SESSION_H__
#define __OPEN_CAROM3D_SERVER_CAROM3D_USER_SESSION_H__

#include <vector>

#include <core/server/server_context.h>
#include <core/server/client_session.h>
#include "crypto.h"
#include "action.h"

namespace core {

    class Server;

    class Carom3DUserSession : public ClientSession {

    protected:
        CryptoContext m_inDataCryptoCtx;
        CryptoContext m_outDataCryptoCtx;
        std::vector<ActionData> m_pendingActions;
        std::vector<u8> m_unparsedData;

    public:
        Carom3DUserSession(nettools::ntConnection& ntConnection, Server& server)
            : ClientSession(ntConnection, server) {

        }

        CryptoContext& inDataCryptoCtx() { return m_inDataCryptoCtx; }
        CryptoContext& outDataCryptoCtx() { return m_outDataCryptoCtx; }

        const std::vector<ActionData>& pendingActions() const { return m_pendingActions; }

        void clearPendingActions() {
            if(!m_pendingActions.empty())
                m_pendingActions.clear();
        }

        void appendActions(const std::vector<ActionData> &actions) {
            m_pendingActions.insert(m_pendingActions.end(), actions.begin(), actions.end());
        }

        void sendAction(const ActionData& action) {
            unsigned char data[8192]; //TODO(CGR): MAX_PACKET_SIZE
            //TODO(CGR): assert data len won't exceed MAX_PACKET_SIZE
            *(int*)&data[0] = action.id();
            *(unsigned int*)&data[4] = action.data().size();
            unsigned int actionDataLen = action.data().size();
            if(actionDataLen > 0) {
                ::memcpy(&data[8], action.data().data(), actionDataLen);
            }
            m_outDataCryptoCtx.crypt(data, actionDataLen + 8);
            m_outDataCryptoCtx.update();
            m_ntClient.send(m_ntClient.socket(), data, actionDataLen + 8);
        }

    };

}

#endif //__OPEN_CAROM3D_SERVER_USER_SESSION_H__
