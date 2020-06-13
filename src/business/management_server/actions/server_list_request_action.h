//
// Created by CGR on 15/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_SERVER_LIST_REQUEST_H__
#define __OPEN_CAROM3D_SERVER_SERVER_LIST_REQUEST_H__

#include "../management_server_action.h"

namespace business { namespace management {

    class ServerListRequestAction : public ManagementServerAction<void> {
    public:
        bool validate(const ActionData &action) override { return true; }

        void execute(const ActionData &action, User &user, const void *unused) override;
    };

}}

#endif //__OPEN_CAROM3D_SERVER_SERVER_LIST_REQUEST_H__
