//
// Created by CGR on 20/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_ACTION_DISPATCHER_H__
#define __OPEN_CAROM3D_SERVER_ACTION_DISPATCHER_H__

#include <list>
#include <core/server/action.h>

namespace core {
    class Destination {
    public:
        virtual void send(std::list<core::ActionData*>& actions) = 0;
    };

    class ActionDispatcher {
    protected:
        std::list<ActionData*> m_actions;
        Destination *m_destination;

    public:
        static ActionDispatcher& prepare();
        ActionDispatcher &action(ActionData& data);
        ActionDispatcher &to(const Destination &destination);
        void send();
    };
}

using core::ActionDispatcher;

#endif //__OPEN_CAROM3D_SERVER_ACTION_DISPATCHER_H__
