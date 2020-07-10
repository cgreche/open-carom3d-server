//
// Created by CGR on 12/06/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_MANAGEMENT_SERVER_ACTION_H__
#define __OPEN_CAROM3D_SERVER_MANAGEMENT_SERVER_ACTION_H__

#include <list>
#include <core/server/carom3d/action.h>

namespace business {

    using core::Action;
    using core::ActionData;
    
    class User;

    template <class T>
    class ManagementServerAction : public core::Action {
    public:
        bool validate(const ActionData& action) {
            return true;
        }

        void execute(const ActionData& action, core::ClientSession& user) override {
            execute(action, (User&)user, cast(action));
        }

        const T* cast(const ActionData& action) {
            return (T*)action.data().data();
        }

        virtual void execute(const ActionData& action, User& user, const T* data) = 0;
    };

}

#endif //__OPEN_CAROM3D_SERVER_MANAGEMENT_SERVER_ACTION_H__
