//
// Created by CGR on 14/05/2020.
//

#include <core/server/client_session.h>
#include "login_action.h"
#include <business/service/account_service.h>
#include <business/entity/player.h>

namespace business { namespace management {

#pragma pack(push, 1)
    struct MSLoginResult {
        unsigned long result;
        wchar_t playerName[21];
        unsigned long totalPoints;
        unsigned long unk;
        unsigned long accountLevel;
        // Probably cue and jewels stats
        unsigned long _unk;
        unsigned long _unk1;
        unsigned char _unk2[6];
        unsigned long _unk3;
        unsigned char _unk4;
        unsigned char _unk5;
        unsigned char _unk6[8];
        unsigned short _unk7;
        //
        wchar_t ip[16];
        unsigned long unk2;
        unsigned long unk3;
        unsigned long unk4;
        unsigned long unk5;
        unsigned long unk6;
        unsigned long unk7;
    };
#pragma pack(pop)

    void sendMSLoginResult(u32 result, User& user) {
        Player* player = user.player();

        MSLoginResult loginResult = {
            result,
            L"",
            (unsigned long)player->points(), 0, (unsigned long)1,
            0, 0, {0, 0, 0, 0, 0, 0}, 0, 0, 0,
            {0, 0, 0, 0, 0, 0, 0, 0},
            0,
            L"127.0.0.1",
            0, 0, 0, 0, 0, 0
        };

        ::wcsncpy(loginResult.playerName, player->name(), 20);
        loginResult.playerName[20] = L'\x00';

        ActionData actionData(0x05, (unsigned char*)&loginResult, sizeof(loginResult));
        user.client().sendAction(actionData);
    }

    bool LoginAction::validate(const ActionData &action) {
        int size = action.data().size();
        int _size = sizeof(LoginData);
        return size == _size;
    }

    void LoginAction::execute(const ActionData &action, User &user, const LoginData *data) {
        printf("Player logged in: %ws, %ws", data->username, data->password);

        Account* account = AccountService::getInstance().logAccount(data->username, data->password);
        Player* player;
        if(nullptr == account) {
            account = AccountService::getInstance().createAccount(data->username, data->password);
            player = AccountService::getInstance().createPlayerFromAccount(*account);
        }
        else {
            player = AccountService::getInstance().getPlayer(*account);
        }

        account->setPlayer(player);

        user.setAccount(account);
        user.setPlayer(player);

        sendMSLoginResult(0x00, user);
    }

}}
