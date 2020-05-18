//
// Created by CGR on 15/05/2020.
//

#include <business/account.h>
#include <business/lobby_service.h>
#include <business/user_service.h>
#include "actions/channel_message_action.h"
#include "actions/login_action.h"
#include "game_server.h"

namespace game {

#define LOGIN_ACTION 0x01
#define CHANNEL_MESSAGE_ACTION 0x04

    static std::map<int, Action *> actions = {
            {LOGIN_ACTION, new LoginAction},
            {CHANNEL_MESSAGE_ACTION, new ChannelMessageAction},
    };

    GameServer::GameServer(const ServerConfig &config)
            : Server(config) {
        this->setActionMap(&game::actions);
    }

    void GameServer::onClientConnection(ClientSession *client) {
        Server::onClientConnection(client);
    }

    void GameServer::onClientDisconnection(ClientSession *client) {
        Server::onClientDisconnection(client);
        User* user = UserService::getInstance().getUser(*client);
        LobbyService::getInstance().removeUserFromPlace(user);
        UserService::getInstance().logoutUser(*user);
    }

}