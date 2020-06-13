//
// Created by CGR on 14/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_LOGINACTION_H__
#define __OPEN_CAROM3D_SERVER_LOGINACTION_H__

#include "../management_server_action.h"

namespace business { namespace management {

#pragma pack(push, 1)
    struct LoginData {
        const wchar_t username[21];
        const wchar_t password[13];
        const wchar_t country[4];
        const unsigned char gpuInfo[144]; //TODO: wchar_t?
        int unk;
    };
#pragma pack(pop)

    class LoginAction : public ManagementServerAction<LoginData> {

    public:
        bool validate(const ActionData &action);

        void execute(const ActionData &action, User &user, const LoginData *data) override;

    };

}}

#endif //__OPEN_CAROM3D_SERVER_LOGINACTION_H__
