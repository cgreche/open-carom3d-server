//
// Created by CGR on 15/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_CLIENT_VERSION_ACTION_H__
#define __OPEN_CAROM3D_SERVER_CLIENT_VERSION_ACTION_H__

#include "../management_server_action.h"

namespace business { namespace management {

    class ClientVersionAction : public ManagementServerAction<const char> {
    public:
        bool validate(const ActionData &action) override;

        void execute(const ActionData &action, User &user, const char *data) override;
    };

}}

#endif //__OPEN_CAROM3D_SERVER_CLIENT_VERSION_ACTION_H__
