//
// Created by CGR on 15/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_CLIENT_VERSION_ACTION_H__
#define __OPEN_CAROM3D_SERVER_CLIENT_VERSION_ACTION_H__

#include "core/server/action.h"

namespace management {

    class ClientVersionAction : public AbstractAction<const char> {
    public:
        bool validate(ActionData &action) override;

        void execute(ActionData &action, ClientSession &client, const char *data) override;
    };

}

#endif //__OPEN_CAROM3D_SERVER_CLIENT_VERSION_ACTION_H__
