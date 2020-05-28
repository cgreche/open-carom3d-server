//
// Created by CGR on 14/05/2020.
//

#include "server_context.h"
#include "client_session.h"

namespace core {

    Server::Server(const ServerConfig &config)
        : m_messageListener(*this) {
        m_config = config;
        m_actionMap = nullptr;

        m_messageListener.listen(config.port);
    }

    void Server::poll() {
        m_messageListener.poll();
    }

    void Server::run() {
        for(;;) {
            m_messageListener.poll();
        }
    }

    void Server::addClient(ClientSession *client) {
        //assert(client != nullptr_t);
        m_clients.push_back(client);
    }

    void Server::removeClient(ClientSession *client) {
        for(auto it = m_clients.begin(); it != m_clients.end(); ++it) {
            if (*it == client) {
                m_clients.erase(it);
                return;
            }
        }
    }

    void Server::processClientActions(ClientSession *client) {
        if(m_actionMap == nullptr)
            return;

        const std::vector<ActionData>& actions = client->pendingActions();
        if(actions.empty())
            return;

        for(auto actionData : actions) {
            Action* action = (*m_actionMap)[actionData.id()];
            if(action != nullptr) {
                if (action->validate(actionData)) {
                    action->execute(actionData, *client);
                }
            }
            else {
                //TODO: Invalid Action
                int a = actionData.id();
                printf("Unhandled action: %x - %d\n", a, actionData.data().size());
            }
        }
        client->clearPendingActions();
    }

    void Server::disconnectClient(ClientSession *client) {
        //TODO
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