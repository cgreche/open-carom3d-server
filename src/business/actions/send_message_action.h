//
// Created by CGR on 14/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_SENDMESSAGEACTION_H__
#define __OPEN_CAROM3D_SERVER_SENDMESSAGEACTION_H__

#include "../../core/server/action.h"

//TODO
struct MessageData {
    const char* message;
};

class send_message_action : public AbstractAction<MessageData> {
    virtual bool validate(ActionData) {
        return true;
    }

    void execute(ActionData &action, ClientSession &client, const MessageData* data) override;
};


#endif //__OPEN_CAROM3D_SERVER_SENDMESSAGEACTION_H__
