//
// Created by CGR on 14/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_GAME_SERVER_LOGINACTION_H__
#define __OPEN_CAROM3D_SERVER_GAME_SERVER_LOGINACTION_H__

#include <business/util/abstract_action.h>
#include <business/api/messaging.h>

namespace business {

    class LoginAction : public AbstractAction<LoginActionData> {
    public:
        bool validate(const ActionData &action) override;
        void execute(const ActionData &action, User &user, const LoginActionData *data) override;
    };

}

#endif //__OPEN_CAROM3D_SERVER_LOGINACTION_H__
