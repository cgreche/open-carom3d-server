//
// Created by CGR on 14/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_LOGINACTION_H__
#define __OPEN_CAROM3D_SERVER_LOGINACTION_H__

#include "../../core/server/action.h"

struct LoginData {
    const wchar_t username[21];
    const wchar_t password[13];
    const wchar_t country[4];
    const unsigned char gpuInfo[144]; //TODO: wchar_t?
    int unk;
};

class LoginAction : public AbstractAction<LoginData> {

public:
    bool validate(ActionData &action);
    void execute(ActionData &action, ClientSession &client, const LoginData* data) override;

};

#endif //__OPEN_CAROM3D_SERVER_LOGINACTION_H__
