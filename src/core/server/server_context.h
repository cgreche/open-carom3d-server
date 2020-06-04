//
// Created by CGR on 14/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_SERVER_CONTEXT_H__
#define __OPEN_CAROM3D_SERVER_SERVER_CONTEXT_H__

#include <map>
#include "server_config.h"
#include "messaging/message_listener.h"

namespace core {

    class ClientSession;

    class Action;

    class Server {
        ServerConfig m_config;
        MessageListener m_messageListener;

        std::vector<ClientSession *> m_clients;

        std::map<int, Action *> *m_actionMap;

        void addClient(ClientSession *client);

        void removeClient(ClientSession *client);

    public:
        //TODO: What is explicit?
        explicit Server(const ServerConfig &config);

        void poll();

        virtual void run();

        void processClientActions(ClientSession *client);

        void sendAction(Action &action);

        void disconnectClient(ClientSession *client);

        void setActionMap(std::map<int, Action *> *actionMap);

        virtual void onClientConnection(ClientSession* client);
        virtual void onClientAction(ClientSession* client, const ActionData& actionData);
        virtual void onUnhandledClientAction(ClientSession* client, const ActionData& actionData);
        virtual void onClientDisconnection(ClientSession *client);

        const char *hostname() const { return m_config.host.c_str(); }

        unsigned short port() const { return m_config.port; }

        virtual unsigned int clientsConnectedCount() {
            return m_clients.size();
        }

    };

}

#endif //__OPEN_CAROM3D_SERVER_SERVER_CONTEXT_H__
