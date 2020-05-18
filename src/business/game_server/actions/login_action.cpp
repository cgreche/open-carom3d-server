//
// Created by CGR on 14/05/2020.
//

#include <core/server/client_session.h>
#include <business/account_service.h>
#include <business/lobby_service.h>
#include <business/user_service.h>
#include "login_action.h"

#include <thread>

namespace game {

#pragma pack(push, 1)
    struct LoginResult {
        long result;
        wchar_t playerName[21];
        unsigned long accountNumber;
        unsigned long accountLevel;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct ChannelPlayer {
        unsigned long accountNumber;
        wchar_t playerName[21];
        bool inGuild;
        unsigned long level;
        unsigned long highestRating;
        unsigned long wins; //related to stars
        unsigned long chalks;
        unsigned long unk;
        bool hasPicture;
        wchar_t country[3];
        long caneys;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct PlayerStats {
        unsigned short doublePointsItemCount;
        unsigned long vipItemTime;
        unsigned char unk06;
        unsigned char power;
        unsigned char powerRange;
        unsigned char control;
        unsigned char sideSpin;
        unsigned char topSpin;
        unsigned char bottomSpin;
        unsigned char ps;
        unsigned char ms;
        short availableStatPoints;
        unsigned long unk11;
        unsigned long cueId;
        unsigned char jewel_ruby;
        unsigned char jewel_zaphira;
        unsigned char jewel_opal;
        unsigned char jewel_emerald;
        unsigned char jewel_onix;
        unsigned char jewel_topaz;
        unsigned long unk1f;
        unsigned char unk23;
        unsigned char resetStatsItemCount;
        unsigned short doubleCaneysItemCount;
        int caneys;
        wchar_t country[3];
        //TODO(CGR): correct?
    };
#pragma pack(pop)


    void sendClientVersion(ClientSession &client) {
        char version[20] = "5.31";
        ActionData action(0x16, (unsigned char *) version, sizeof(version));
        client.sendAction(action);
    }

    void sendLoginResult(User* user) {
        LoginResult loginResult = {
                0,
                L"",
                (unsigned long)user->account().id(),
                100
        };
        ::wcsncpy(loginResult.playerName, user->player().name(), 20);
        loginResult.playerName[20] = L'\0';

        ActionData loginResultAction(0x18, (unsigned char *)&loginResult, sizeof(loginResult));
        user->client().sendAction(loginResultAction);
    }

    void sendBlackBoardMessage(ClientSession &client, const wchar_t *message) {
        ActionData blackBoardMessageAction(0x56, (unsigned char *) message, (::wcslen(message)+1)*2);
        client.sendAction(blackBoardMessageAction);
    }

    void sendElectronicPanelMessage(ClientSession &client, const wchar_t *message) {
        ActionData action(0x54, (unsigned char *) message, (::wcslen(message)+1)*2);
        client.sendAction(action);
    }

    void sendNotifyMessage(ClientSession &client, const wchar_t *message) {
        ActionData greenMessage(0x1C, (unsigned char *) message, (::wcslen(message)+1)*2);
        client.sendAction(greenMessage);
    }

    void joinChannel(ClientSession &client, const wchar_t* channelName) {
        User* user = UserService::getInstance().getUser(client);
        Channel * channel = LobbyService::getInstance().createChannel(channelName);
        LobbyService::getInstance().moveUserToChannel(user, channelName, true);

        ActionData joinChannelAction(0x1D, (unsigned char *) channelName, (::wcslen(channelName)+1) * 2);
        client.sendAction(joinChannelAction);

        ChannelPlayer channelPlayer = {0};
        channelPlayer.accountNumber = user->account().id();
        ::wcsncpy(channelPlayer.playerName, user->account().name(),20);
        channelPlayer.playerName[20] = L'\0';
        channelPlayer.level = 1;

        for(User* userIn : channel->usersIn()) {
            if(user == userIn)
                continue;
            ActionData channelUserAction(0x1E, (unsigned char*)&channelPlayer, sizeof(channelPlayer));
            userIn->client().sendAction(channelUserAction);
        }

        //TODO(CGR): One or more of:
        // ActionData roomInfo(0x2B, (unsigned char*)nullptr, 0);
        // ActionData roomListEnd(0x62, (unsigned char*)nullptr, 0);

        for(User* user : channel->usersIn()) {
            ChannelPlayer channelPlayer = {0};
            channelPlayer.accountNumber = user->account().id();
            channelPlayer.level = 1;
            ::wcscpy(channelPlayer.playerName, user->player().name());
            ActionData channelPlayerAction(0x1E, (unsigned char*)&channelPlayer, sizeof(channelPlayer));
            client.sendAction(channelPlayerAction);
        }

    }

    bool LoginAction::validate(ActionData &action) {
        int size = action.data().size();
        int _size = sizeof(LoginData);
        return size == _size;
    }

    void LoginAction::execute(ActionData &action, ClientSession &client, const LoginData *data) {
        printf("Player logged in: %ws, %ws", data->username, data->password);

        Account* account = AccountService::getInstance().logAccount(client, data->username, data->password);
        User* user = UserService::getInstance().createUserSession(client, *account, *account->player());

        sendClientVersion(client);

        int waitValue = 0;
        ActionData notifyRemainAction(0x60, (unsigned char*)&waitValue, 4);
        client.sendAction(notifyRemainAction);

        //TODO(CGR): sleeping is bad. But without it, messages aren't processed correctly on client
        // Find a better way to do it
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        sendLoginResult(user);

        //TODO(CGR): process login result
        // If login result == 0
        sendBlackBoardMessage(client, L"Welcome to Open Carom3D (v 0.1.0 Alpha)");
        sendElectronicPanelMessage(client, L"Hello World");

        PlayerStats playerStats = {0};
        ActionData playerStatsAction(0x68, (unsigned char*)&playerStats, sizeof(playerStats));
        client.sendAction(playerStatsAction);

        sendNotifyMessage(client, L"Welcome to Open Carom3D Server\n");

        std::wstring generatedChannelName = L"Carom ";
        generatedChannelName += data->country;
        generatedChannelName += L"-1";
        joinChannel(client, generatedChannelName.c_str());

    }

}
