//
// Created by CGR on 13/05/2020.
//

#include <nettools/ntClient.h>
#include "client_session.h"

namespace core {

    ClientSession::ClientSession(nettools::ntConnection &ntClient, Server &mServer)
            : m_ntClient(ntClient), m_server(mServer) {
        m_sessionId = ntClient.socket();
    }

}