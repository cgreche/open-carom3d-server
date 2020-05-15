//
// Created by CGR on 13/05/2020.
//

#include <nettools/ntClient.h>
#include "client_session.h"

ClientSession::ClientSession(nettools::ntClient &ntClient, Server &mServer)
    : m_ntClient(ntClient), m_server(mServer) {
}

void ClientSession::sendAction(ActionData &action) {
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
    m_ntClient.send(data, actionDataLen + 8);
}