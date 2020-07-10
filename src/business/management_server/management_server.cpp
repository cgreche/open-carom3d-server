//
// Created by CGR on 15/05/2020.
//

#include <map>
#include <business/entity/user.h>
#include <business/entity/player.h>
#include <core/server/carom3d/carom3d_protocol.h>
#include "actions/client_version_action.h"
#include "actions/server_list_request_action.h"

#include "actions/login_action.h"
#include "management_server.h"

namespace business { namespace management {

#define SERVER_LIST_REQUEST 0x00
#define LOGIN_ACTION 0x01
#define CLIENT_VERSION 0x65

    using core::Action;

    static std::map<int, Action *> actions = {
            {SERVER_LIST_REQUEST, new ServerListRequestAction},
            {LOGIN_ACTION,        new LoginAction},
            {CLIENT_VERSION,      new ClientVersionAction}
    };

    class ManagementServerProtocol : public core::Carom3DProtocol {
    public:
        ManagementServerProtocol() {
            this->setUserActionMap(&actions);
        }

        core::ClientSession* createSession(nettools::ntConnection& ntClient, core::Server& server) {
            return new User(ntClient, server);
        }

        void onUnhandledUserAction(core::Carom3DUserSession& session, const ActionData& actionData) {
            //TODO: Invalid Action
            User& user = (User&)session;
            if(!user.player())
                core::Carom3DProtocol::onUnhandledUserAction(session, actionData);
            else
                printf("Unhandled client action: %S - %x - %d\n",
                    user.player()->name(),
                    actionData.id(),
                    actionData.data().size());
        }
    };

    static ManagementServerProtocol protocol;

    core::MessagingProtocol* ManagementServer::messagingProtocol() {
        return &protocol;
    }


    ManagementServer::ManagementServer(const core::ServerConfig &config)
            : core::Carom3DServer(config) {
    }

    void ManagementServer::onClientConnection(core::ClientSession& client) {
        Server::onClientConnection(client);

        char version[] = "5.31";
        ActionData versionAction(0x00, (unsigned char*)version, sizeof(version));
        ((User&)client).sendAction(versionAction);
    }

    void ManagementServer::onClientDisconnection(core::ClientSession& client) {
        //TODO(CGR)
    }

}}