//
// Created by CGR on 14/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_SERVER_CONTEXT_H__
#define __OPEN_CAROM3D_SERVER_SERVER_CONTEXT_H__

#include <map>

#include "server_config.h"
#include "messaging/message_listener.h"

class ClientSession;
class Action;

class Server {
    ServerConfig m_config;
    MessageListener m_messageListener;

    std::vector<ClientSession*> m_clients;

    std::map<int, Action*>* m_actionMap;

public:
    //TODO: What is explicit?
    explicit Server(const ServerConfig &config);

    void run();

    void addClient(ClientSession* client);
    void processClientActions(ClientSession* client);
    void sendAction(Action& action);
    void disconnectClient(ClientSession* client);

    void setActionMap(std::map<int, Action*>* actionMap);
};

#endif //__OPEN_CAROM3D_SERVER_SERVER_CONTEXT_H__
