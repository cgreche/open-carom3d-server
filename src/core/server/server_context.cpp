//
// Created by CGR on 14/05/2020.
//

#include "server_context.h"
#include "client_session.h"

namespace core {

    static class EventHandler : public nettools::ntEventHandler {
        Server& m_server;
        MessageParser m_messageParser;

    public:
        EventHandler(Server& server)
            : m_server(server) {
        }

        void onClientConnection(nettools::ntClient* ntClient) override {
            ClientSession* client = new ClientSession(*ntClient, m_server);
            ntClient->bindData(client);
            m_server.onClientConnection(client);
        }

        void onClientDisconnection(nettools::ntClient* ntClient) override {
            ClientSession* client = (ClientSession*)ntClient->bindedData();
            m_server.onClientDisconnection(client);
        }

        void onClientDataReceived(nettools::ntClient* ntClient, unsigned char* data, unsigned int dataLen) override {
            ClientSession* client = (ClientSession*)ntClient->bindedData();
            ParsedDataResultInfo parsedData = m_messageParser.parseMessageData(client->inDataCryptoCtx(), data, dataLen);

            unsigned int parsedLen = parsedData.parsedTotalLen;
            if(parsedLen < dataLen)
                client->appendUnparsedData(&data[parsedLen], dataLen - parsedLen);
            client->appendActions(parsedData.parsedActions);
            m_server.processClientActions(client);
        }

        void onClientDataSent(nettools::ntClient* client, const unsigned char* data, unsigned int len) override {
            //TODO
        }
    };


    Server::Server(const ServerConfig &config) {
        m_config = config;
        m_actionMap = nullptr;

        m_ntServer.setEventHandler(new EventHandler(*this));
        m_ntServer.listen(config.port);
    }

    Server::~Server() {
        delete m_ntServer.eventHandler();
    }

    void Server::poll() {
        m_ntServer.poll();
    }

    void Server::run() {
        for(;;) {
            m_ntServer.poll();
        }
    }

    void Server::addClient(ClientSession *client) {
        //assert(client != nullptr_t);
        m_clients[client->sessionId()] = client;
    }

    void Server::removeClient(ClientSession *client) {
        m_clients.erase(client->sessionId());
    }

    void Server::processClientActions(ClientSession *client) {
        if(m_actionMap == nullptr)
            return;

        const std::vector<ActionData>& actions = client->pendingActions();
        if(actions.empty())
            return;

        for(auto actionData : actions) {
            onClientAction(client, actionData);
        }
        client->clearPendingActions();
    }

    void Server::onClientAction(ClientSession* client, const ActionData& actionData) {
        Action* action = (*m_actionMap)[actionData.id()];
        if(action != nullptr) {
            if(action->validate(actionData)) {
                action->execute(actionData, *client);
            }
        }
        else {
            onUnhandledClientAction(client, actionData);
        }
    }

    void Server::onUnhandledClientAction(ClientSession *client, const ActionData& actionData) {
        //TODO: Invalid Action
        int a = actionData.id();
        printf("Unhandled action: %x - %d\n", a, actionData.data().size());
    }


    void Server::disconnectClient(ClientSession *client) {
        m_ntServer.disconnect(&client->ntClient());
    }

    void Server::setActionMap(std::map<int, Action*>* actionMap) {
        m_actionMap = actionMap;
    }

    void Server::onClientConnection(ClientSession *client) {
        addClient(client);
    }

    void Server::onClientDisconnection(ClientSession *client) {
        removeClient(client);
    }

}