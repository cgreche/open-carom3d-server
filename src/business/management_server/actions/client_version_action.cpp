//
// Created by CGR on 15/05/2020.
//

#include "client_version_action.h"

namespace management {

    bool ClientVersionAction::validate(ActionData &action) {
        //TODO(CGR): validate version
        return true;
    }

    void ClientVersionAction::execute(ActionData &action, ClientSession &client, const char *data) {
        printf(data);
    }

}