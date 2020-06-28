//
// Created by CGR on 15/05/2020.
//

#include <list>
#include <thread>

#include <core/server/server_context.h>
#include <business/entity/user.h>
#include <business/game_server/game_server.h>
#include <business/game_server/game_server_config.h>
#include <business/service/server_service.h>
#include <core/server/carom3d/util/action_builder.h>
#include "server_list_request_action.h"

namespace business { namespace management {

#pragma pack(push, 1)
    struct ServerInfo {
        unsigned long serverType; //+0x00
        unsigned long serverId; //+0x04
        wchar_t serverName[41]; //+0x08
        unsigned long serverNumber; //+0x5A
        char serverHost[16]; //+0x5E
        unsigned long port; //+0x6E
        unsigned long maxPlayers; //+0x72
        unsigned long minPoints; //+0x76
        unsigned long maxPoints; //+0x7A
        unsigned long unk1; //+0x7E
        unsigned long unk2; //+0x82
        unsigned long playersConnected; //+0x86
        unsigned long serverState; //+0x8A
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

    void ServerListRequestAction::execute(const ActionData &action, User &user, const void *unused) {

        ServerInfo serverInfo = {
                0,
                0,
                L"",
                0x3,
                "",
                0,
                500,
                0,
                500,
                0, //unk
                100, //unk
                0,
                1
        };

        ServerInfoUpdate updateInfo = {0};
        updateInfo.serverState = 1;

        std::list<core::Server*> servers = business::ServerService::getInstance().getServerList();
        for(core::Server* server : servers) {
            const business::GameServerConfig& config = ((const GameServer*)server)->gsConfig;
            serverInfo.serverType = config.eventType;
            serverInfo.serverId = config.serverId;
            serverInfo.serverNumber = config.serverTemplate.templateId;
            serverInfo.minPoints = config.serverTemplate.minPoints;
            serverInfo.maxPoints = config.serverTemplate.maxPoints;
            ::wcsncpy(serverInfo.serverName, ((const GameServer*)server)->qualifiedName().data(), 40);
            serverInfo.serverName[40] = L'\0';
            ::strncpy(serverInfo.serverHost, config.hostInfo.hostname.c_str(), 15);
            serverInfo.serverHost[15] = '\0';
            serverInfo.port = config.hostInfo.port;
            serverInfo.playersConnected = server->clientsConnectedCount();

            ActionData serverInfoAction(0x01, (unsigned char *) &serverInfo, sizeof(serverInfo));
            user.sendAction(serverInfoAction);

            ActionData serverListEnd(0x04, nullptr, 0);
            user.sendAction(serverListEnd);
        }


    }

}}