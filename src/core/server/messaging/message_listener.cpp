//
// Created by CGR on 13/05/2020.
//

#include "message_listener.h"
#include "../server_context.h"
#include "../client_session.h"

void MessageListener::onClientConnection(nettools::ntClient *ntClient) {
    ClientSession *client = new ClientSession(*ntClient, m_serverCtx);
    ntClient->bindData(client);
    m_serverCtx.addClient(client);
}

void MessageListener::onClientDisconnection(nettools::ntClient *ntClient) {
    //TODO
}

void MessageListener::onClientDataReceived(nettools::ntClient *ntClient, unsigned char *data, unsigned int dataLen) {
    ClientSession *client = (ClientSession*)ntClient->bindedData();
    ParsedDataResultInfo parsedData = this->m_messageParser.parseMessageData(client->inDataCryptoCtx(), data, dataLen);

    unsigned int parsedLen = parsedData.parsedTotalLen;
    if(parsedLen < dataLen)
        client->appendUnparsedData(&data[parsedLen], dataLen - parsedLen);
    client->appendActions(parsedData.parsedActions);
    m_serverCtx.processClientActions(client);
}

void MessageListener::onClientDataSent(nettools::ntClient *ntClient, const unsigned char *data, unsigned int dataLen) {
    // TODO
}

MessageListener::MessageListener(Server &server)
    : m_serverCtx(server) {
}

void MessageListener::listen(unsigned short port) {
    Server &thisServer = const_cast<Server &>(m_serverCtx);
    static auto _eventHandler = [this, &thisServer](nettools::NT_EVENT event, nettools::ntConnection *connection, unsigned char *data, unsigned int dataLen) {
        nettools::ntClient *ntClient = (nettools::ntClient *) connection;

        switch (event) {
            case nettools::NT_EVT_CONNECT: {
                this->onClientConnection(ntClient);
                break;
            }

            case nettools::NT_EVT_DISCONNECT: {
                this->onClientConnection(ntClient);
                break;
            }

            case nettools::NT_EVT_DATARECV: {
                this->onClientDataReceived(ntClient, data, dataLen);
                break;
            }

            case nettools::NT_EVT_DATASENT: {
                this->onClientDataSent(ntClient, data, dataLen);
                break;
            }
        }
    };

    static auto eventHandler = [](nettools::NT_EVENT event, nettools::ntConnection *connection, unsigned char *data, unsigned int dataLen) {
        return _eventHandler(event, connection, data, dataLen);
    };

    m_ntServer.listen(port);
    m_ntServer.setEventHandler(eventHandler);
    while(m_ntServer.poll() == nettools::NTERR_SUCCESS) {
    }
}

