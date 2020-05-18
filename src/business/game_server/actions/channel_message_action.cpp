//
// Created by CGR on 17/05/2020.
//

#include <business/lobby_service.h>
#include <business/account_service.h>
#include <business/user_service.h>
#include "channel_message_action.h"

namespace game {
    bool ChannelMessageAction::validate(ActionData &action) {
        return true;
    }

    void ChannelMessageAction::execute(ActionData &action, ClientSession &client, const wchar_t *message) {
        User* user = UserService::getInstance().getUser(client);
        LobbyService::getInstance().sendMessage(user, message);
    }
}
