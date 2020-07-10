//
// Created by CGR on 20/05/2020.
//

#include "action_dispatcher.h"

namespace core {

    static ActionDispatcher g_ActionDispatcher;

    ActionDispatcher& ActionDispatcher::prepare() {
        ActionDispatcher &dispatcher = g_ActionDispatcher;
        if(!dispatcher.m_actions.empty())
            dispatcher.m_actions.clear();
        return dispatcher;
    }

    ActionDispatcher &ActionDispatcher::action(const ActionData &data) {
        m_actions.push_back(&data);
        return *this;
    }

    void ActionDispatcher::send(const Destination& destination) {
        ((Destination&)destination).send(m_actions);
    }

}
