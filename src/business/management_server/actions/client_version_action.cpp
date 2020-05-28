//
// Created by CGR on 15/05/2020.
//

#include "client_version_action.h"

namespace business { namespace management {

    bool ClientVersionAction::validate(const ActionData &action) {
        //TODO(CGR): validate version
        return true;
    }

    void ClientVersionAction::execute(const ActionData &action, User &user, const char *data) {
        printf(data);
    }

}}