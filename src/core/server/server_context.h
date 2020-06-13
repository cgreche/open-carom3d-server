//
// Created by CGR on 14/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_SERVER_CONTEXT_H__
#define __OPEN_CAROM3D_SERVER_SERVER_CONTEXT_H__

#include <map>
#include <nettools/ntConnection.h>
#include <nettools/ntServer.h>
#include "server_config.h"
#include "messaging_protocol.h"
#include "client_session.h"
#include "server_context.h"

namespace core {

    class ClientSession;

    class Action;

    class Server {
    protected:
        ServerConfig m_config;
        nettools::ntServer m_ntServer;

        std::map<unsigned long, ClientSession*> m_clients;

        void addClient(ClientSession& client);
        void removeClient(ClientSession& client);

    public:
        explicit Server(const ServerConfig &config);
        virtual ~Server();

        virtual MessagingProtocol* messagingProtocol() = 0;

        virtual void poll();
        virtual void run();

        virtual void disconnectClient(ClientSession *client);

        virtual void onClientConnection(ClientSession& client);
        virtual void onClientDataReceived(ClientSession& client);
        virtual void onClientDataSent(ClientSession& client);
        virtual void onClientDisconnection(ClientSession& client);

        const char *hostname() const { return m_config.host.c_str(); }
        unsigned short port() const { return m_config.port; }

        virtual ClientSession* client(unsigned long sessionId) {
            auto it = m_clients.find(sessionId);
            return it != m_clients.end() ? it->second : nullptr;
        }

        std::map<unsigned long, ClientSession*>& clients() { return m_clients; }
        virtual unsigned int clientsConnectedCount() { return m_clients.size(); }

    };

}

#endif //__OPEN_CAROM3D_SERVER_SERVER_CONTEXT_H__
