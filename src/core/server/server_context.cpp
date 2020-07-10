//
// Created by CGR on 14/05/2020.
//

#include <thread>
#include <nettools/ntEventHandler.h>
#include <nettools/ntClient.h>
#include "server_context.h"
#include "client_session.h"
#include "messaging_protocol.h"

namespace core {

    class EventHandler : public nettools::ntEventHandler {
        Server& m_server;

    public:
        EventHandler(Server& server)
            : m_server(server) {
        }

        void onClientConnection(nettools::ntClient* ntClient) override {
            ClientSession* client = m_server.messagingProtocol()->createSession(*ntClient, m_server);
            ntClient->bindData(client);
            m_server.onClientConnection(*client);
        }

        void onClientDisconnection(nettools::ntClient* ntClient) override {
            ClientSession* client = m_server.client(ntClient->socket());
            m_server.onClientDisconnection(*client);
            m_server.messagingProtocol()->closeSession(*client);
        }

        void onClientDataReceived(nettools::ntClient* ntClient, unsigned char* data, unsigned int dataLen) override {
            ClientSession* client = m_server.client(ntClient->socket());
            client->appendReceivedData(data, dataLen);
            m_server.messagingProtocol()->onMessageReceived(*client);
        }

        void onClientDataSent(nettools::ntClient* ntClient, const unsigned char* data, unsigned int len) override {
            ClientSession* client = m_server.client(ntClient->socket());
            //TODO
        }
    };


    Server::Server(const ServerConfig &config) {
        m_config = config;
        m_ntServer.setEventHandler(new EventHandler(*this));
        m_ntServer.listen(config.hostInfo.port);
    }

    Server::~Server() {
        //TODO(CGR): Disconnect all clients
        delete m_ntServer.eventHandler();
    }

    void Server::poll() {
        m_ntServer.poll();
    }

    void Server::run() {
        for(;;) {
            m_ntServer.poll();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void Server::addClient(ClientSession& client) {
        //assert(client != nullptr_t);
        m_clients[client.sessionId()] = &client;
    }

    void Server::removeClient(ClientSession& client) {
        m_clients.erase(client.sessionId());
    }

    void Server::disconnectClient(ClientSession *client) {
        m_ntServer.disconnect(&client->ntClient());
    }

    void Server::onClientConnection(ClientSession& client) {
        addClient(client);
    }

    void Server::onClientDataReceived(ClientSession& client) {
        //
    }

    void Server::onClientDataSent(ClientSession& client) {
        //
    }

    void Server::onClientDisconnection(ClientSession& client) {
        removeClient(client);
    }

}