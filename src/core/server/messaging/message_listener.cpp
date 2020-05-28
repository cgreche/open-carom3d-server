//
// Created by CGR on 13/05/2020.
//

#include "message_listener.h"
#include "../server_context.h"
#include "../client_session.h"

namespace core {

    void MessageListener::EventHandler::onClientConnection(nettools::ntClient *ntClient) {
        ClientSession *client = new ClientSession(*ntClient, m_messageListener.m_serverCtx);
        ntClient->bindData(client);
        m_messageListener.m_serverCtx.onClientConnection(client);
    }

    void MessageListener::EventHandler::onClientDisconnection(nettools::ntClient *ntClient) {
        ClientSession *client = (ClientSession *) ntClient->bindedData();
        m_messageListener.m_serverCtx.onClientDisconnection(client);
    }

    void MessageListener::EventHandler::onClientDataReceived(nettools::ntClient *ntClient, unsigned char *data,
                                                             unsigned int dataLen) {
        ClientSession *client = (ClientSession *) ntClient->bindedData();
        ParsedDataResultInfo parsedData = m_messageListener.m_messageParser.parseMessageData(client->inDataCryptoCtx(),
                                                                                             data, dataLen);

        unsigned int parsedLen = parsedData.parsedTotalLen;
        if (parsedLen < dataLen)
            client->appendUnparsedData(&data[parsedLen], dataLen - parsedLen);
        client->appendActions(parsedData.parsedActions);
        m_messageListener.m_serverCtx.processClientActions(client);
    }

    void MessageListener::EventHandler::onClientDataSent(nettools::ntClient *ntClient, const unsigned char *data,
                                                         unsigned int dataLen) {
        //TODO
    }

    MessageListener::MessageListener(Server &server)
            : m_serverCtx(server), m_eventHandler(*this) {
    }

    void MessageListener::listen(unsigned short port) {
        m_ntServer.listen(port);
        m_ntServer.setEventHandler(&m_eventHandler);
    }

    void MessageListener::poll() {
        nettools::NT_ERROR error = m_ntServer.poll();
    }
}
