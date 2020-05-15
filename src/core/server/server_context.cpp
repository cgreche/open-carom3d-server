//
// Created by CGR on 14/05/2020.
//

#include "server_context.h"
#include "client_session.h"

Server::Server(const ServerConfig &config)
    : m_messageListener(*this) {
    m_config = config;
    m_actionMap = nullptr;
}

void Server::run() {
    m_messageListener.listen(m_config.port);

}

void Server::addClient(ClientSession *client) {
    //assert(client != nullptr_t);
    m_clients.push_back(client);
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