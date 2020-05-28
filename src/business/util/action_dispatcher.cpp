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

    ActionDispatcher &ActionDispatcher::action(ActionData &data) {
        m_actions.push_back(&data);
        return *this;
    }

    ActionDispatcher &ActionDispatcher::to(const Destination &destination) {
        m_destination = const_cast<Destination *>(&destination);
        return *this;
    }

    void ActionDispatcher::send() {
        m_destination->send(m_actions);
    }

}
