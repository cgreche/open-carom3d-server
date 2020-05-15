//
// Created by CGR on 14/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_SENDPRIVATEMESSAGEACTION_H__
#define __OPEN_CAROM3D_SERVER_SENDPRIVATEMESSAGEACTION_H__

#include "../../core/server/action.h"

struct PrivateMessageData {
};

class SendPrivateMessageAction : public AbstractAction<PrivateMessageData> {
public:
    void execute(ActionData &action, ClientSession &client, const PrivateMessageData* data) override;
};


#endif //__OPEN_CAROM3D_SERVER_SENDPRIVATEMESSAGEACTION_H__
