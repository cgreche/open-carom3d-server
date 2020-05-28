//
// Created by CGR on 14/05/2020.
//

#include <thread>

#include <core/server/client_session.h>
#include <business/service/account_service.h>
#include <business/service/channel_service.h>
#include <business/service/user_service.h>
#include "login_action.h"

namespace business {

    void sendClientVersion(User &user) {
        char version[20] = "5.31";
        ActionData action(0x16, (unsigned char *) version, sizeof(version));
        user.client().sendAction(action);
    }

    void sendLoginResult(User& user) {
        LoginResult loginResult = {
                0,
                L"",
                (unsigned long)user.account()->id(),
                100
        };
        ::wcsncpy(loginResult.playerName, user.player()->name(), 20);
        loginResult.playerName[20] = L'\0';

        ActionData loginResultAction(0x18, (unsigned char *)&loginResult, sizeof(loginResult));
        user.client().sendAction(loginResultAction);
    }

    void sendBlackBoardMessage(User &user, const wchar_t *message) {
        ActionData blackBoardMessageAction(0x56, (unsigned char *) message, (::wcslen(message)+1)*2);
        user.client().sendAction(blackBoardMessageAction);
    }

    void sendElectronicPanelMessage(User &user, const wchar_t *message) {
        ActionData action(0x54, (unsigned char *) message, (::wcslen(message)+1)*2);
        user.client().sendAction(action);
    }

    void sendNotifyMessage(User &user, const wchar_t *message) {
        ActionData greenMessage(0x1C, (unsigned char *) message, (::wcslen(message)+1)*2);
        user.client().sendAction(greenMessage);
    }

    void joinChannel(User &user, const wchar_t* channelName) {
        UserService::getInstance().joinChannel(user, channelName, true);
        UserService::getInstance().updateUserWithAllServerRooms(user);
    }

    bool LoginAction::validate(const ActionData &action) {
        int size = action.data().size();
        int _size = sizeof(LoginActionData);
        return size == _size;
    }

    void LoginAction::execute(const ActionData &action, User &user, const LoginActionData *data) {
        printf("Player logged in: %ws, %ws", data->username, data->password);

        Account* account = AccountService::getInstance().logAccount(data->username, data->password);
        user.setAccount(account);
        user.setPlayer(account->player());

        sendClientVersion(user);

        int waitValue = 0;
        ActionData notifyRemainAction(0x60, (unsigned char*)&waitValue, 4);
        user.client().sendAction(notifyRemainAction);

        //TODO(CGR): sleeping is bad. But without it, messages aren't processed correctly on client
        // Find a better way to do it
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        sendLoginResult(user);

        //TODO(CGR): process login result
        // If login result == 0
        sendBlackBoardMessage(user, L"Welcome to Open Carom3D (v 0.1.0 Alpha)");
        sendElectronicPanelMessage(user, L"Hello World");

        PlayerStats playerStats = {0};
        playerStats.country[0] = L'B';
        playerStats.country[1] = L'R';
        ActionData playerStatsAction(0x68, (unsigned char*)&playerStats, sizeof(playerStats));
        user.client().sendAction(playerStatsAction);

        sendNotifyMessage(user, L"Welcome to Open Carom3D Server\n");

        std::wstring generatedChannelName = L"Carom ";
        generatedChannelName += data->country;
        generatedChannelName += L"-1";
        joinChannel(user, generatedChannelName.c_str());
    }

}