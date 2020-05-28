//
// Created by CGR on 18/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_JOIN_CHANNEL_ACTION_H__
#define __OPEN_CAROM3D_SERVER_JOIN_CHANNEL_ACTION_H__

#include <business/util/abstract_action.h>

namespace business {

    class JoinChannelAction : public AbstractAction<wchar_t> {
    public:
        void execute(const ActionData &action, User &user, const wchar_t *data) override;
    };

}

#endif //__OPEN_CAROM3D_SERVER_JOIN_CHANNEL_ACTION_H__
