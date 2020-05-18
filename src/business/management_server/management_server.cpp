//
// Created by CGR on 15/05/2020.
//

#include <map>
#include <core/server/client_session.h>
#include "actions/client_version_action.h"
#include "actions/server_list_request_action.h"

#include "actions/login_action.h"
#include "management_server.h"

namespace management {

#define SERVER_LIST_REQUEST 0x00
#define LOGIN_ACTION 0x01
#define CLIENT_VERSION 0x65

    static std::map<int, Action *> actions = {
            {SERVER_LIST_REQUEST, new ServerListRequestAction},
            {LOGIN_ACTION,        new LoginAction},
            {CLIENT_VERSION,      new ClientVersionAction}
    };

    ManagementServer::ManagementServer(const ServerConfig &config)
            : Server(config) {
        this->setActionMap(&actions);
    }

    void ManagementServer::onClientConnection(ClientSession *client) {
        Server::onClientConnection(client);

        char version[] = "5.31";
        ActionData versionAction(0x00, (unsigned char*)version, sizeof(version));
        client->sendAction(versionAction);
    }

    void ManagementServer::run() {
        Server::run();
    }

}