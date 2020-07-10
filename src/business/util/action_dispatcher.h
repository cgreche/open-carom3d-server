//
// Created by CGR on 20/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_ACTION_DISPATCHER_H__
#define __OPEN_CAROM3D_SERVER_ACTION_DISPATCHER_H__

#include <list>
#include <core/server/carom3d/action.h>

namespace core {
    class Destination {
    public:
        virtual void send(const std::list<const core::ActionData*>& actions) = 0;
    };

    class ActionDispatcher {
    protected:
        std::list<const ActionData*> m_actions;

    public:
        static ActionDispatcher& prepare();
        ActionDispatcher &action(const ActionData& data);
        void send(const Destination& destination);
    };
}

using core::ActionDispatcher;

#endif //__OPEN_CAROM3D_SERVER_ACTION_DISPATCHER_H__
