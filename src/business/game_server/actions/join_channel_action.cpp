//
// Created by CGR on 18/05/2020.
//

#include <business/service/user_service.h>
#include "join_channel_action.h"

namespace business {

    void JoinChannelAction::execute(const ActionData &action, User &user, const wchar_t *data) {
        UserService::getInstance().joinChannel(user, data, true);
    }

}
