//
// Created by CGR on 19/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_ABSTRACT_ACTION_H__
#define __OPEN_CAROM3D_SERVER_ABSTRACT_ACTION_H__

#include <core/server/action.h>
#include <core/server/client_session.h>
#include <business/entity/user.h>
#include <business/service/user_service.h>

namespace business {
    using namespace core;

    class User;

    template<class T>
    class AbstractAction : public Action {
    public:
        bool validate(const ActionData &action) override {
            return true;
        }

        void execute(const ActionData &action, core::ClientSession &client) override {
            const T *typedData = reinterpret_cast<const T *>(action.data().data());
            User* user = UserService::getInstance().getUser(client);
            execute(action, *user, typedData);
        }

        T *cast(ActionData &action) {
            return (T *) action.data();
        }

        virtual void execute(const ActionData &action, User &user, const T *data) = 0;
    };

}

#endif //__OPEN_CAROM3D_SERVER_ABSTRACT_ACTION_H__
