//
// Created by CGR on 15/05/2020.
//

#include <list>
#include <thread>

#include "server_list_request_action.h"
#include <core/server/server_context.h>
#include <core/server/client_session.h>
#include <business/server_service.h>

namespace management {

#pragma pack(push, 1)
    struct ServerInfo {
        unsigned long serverType;
        unsigned long serverId;
        wchar_t serverName[41];
        unsigned long serverNumber;
        char serverHost[16];
        unsigned long port;
        unsigned long maxPlayers;
        unsigned long minPoints;
        unsigned long maxPoints;
        unsigned long unk1;
        unsigned long unk2;
        unsigned long playersConnected;
        unsigned long serverState;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct ServerInfoUpdate {
        unsigned long serverId;
        unsigned long playersConnected;
        unsigned long serverState;
        char serverIp[16];
    };
#pragma pack(pop)

    void ServerListRequestAction::execute(ActionData &action, ClientSession &client, const void *unused) {

        ServerInfo serverInfo = {
                0x01,
                0x02,
                L"Carom PTB-1",
                0x02,
                "",
                9883,
                100,
                0,
                500,
                0, //unk
                100000000, //unk
                0,
                1
        };

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        ServerInfoUpdate updateInfo = {0};
        updateInfo.serverState = 1;

        std::list<Server*> servers = business::ServerService::getInstance().getServerList();
        int i = 1;
        for(Server* server : servers) {
            serverInfo.serverId = i++;
            ::strncpy(serverInfo.serverHost, server->hostname(), 15);
            serverInfo.serverHost[15] = '\0';

            serverInfo.playersConnected = server->clientsConnectedCount();
            ActionData serverInfoAction(0x01, (unsigned char *) &serverInfo, sizeof(serverInfo));
            client.sendAction(serverInfoAction);
        }
        ActionData serverListEnd(0x04, nullptr, 0);
        client.sendAction(serverListEnd);
    }

}