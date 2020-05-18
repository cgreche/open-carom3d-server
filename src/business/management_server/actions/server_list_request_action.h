//
// Created by CGR on 15/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_SERVER_LIST_REQUEST_H__
#define __OPEN_CAROM3D_SERVER_SERVER_LIST_REQUEST_H__

#include <core/server/action.h>

namespace management {

    class ServerListRequestAction : public AbstractAction<void> {
    public:
        bool validate(ActionData &action) override { return true; }

        void execute(ActionData &action, ClientSession &client, const void *unused) override;
    };

}

#endif //__OPEN_CAROM3D_SERVER_SERVER_LIST_REQUEST_H__
