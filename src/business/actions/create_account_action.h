//
// Created by CGR on 14/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_CREATEACCOUNTACTION_H__
#define __OPEN_CAROM3D_SERVER_CREATEACCOUNTACTION_H__

#include "../../core/server/action.h"

struct CreateAccountData {
    //TODO
};

class CreateAccountAction : public AbstractAction<CreateAccountData> {
public:
    CreateAccountAction() = default;
    void execute(ActionData &action, ClientSession &client, const CreateAccountData* data) override;
};


#endif //__OPEN_CAROM3D_SERVER_CREATEACCOUNTACTION_H__
