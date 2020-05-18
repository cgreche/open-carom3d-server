//
// Created by CGR on 14/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_GAME_SERVER_CHANNEL_MESSAGE_ACTION_H__
#define __OPEN_CAROM3D_SERVER_GAME_SERVER_CHANNEL_MESSAGE_ACTION_H__

#include <core/server/action.h>

namespace game {

    class ChannelMessageAction : public AbstractAction<wchar_t> {
    public:
        bool validate(ActionData &action) override;
        void execute(ActionData &action, ClientSession &client, const wchar_t *data) override;
    };

}

#endif //__OPEN_CAROM3D_SERVER_GAME_SERVER_CHANNEL_MESSAGE_ACTION_H__
