//
// Created by CGR on 13/05/2020.
//

#include <thread>
#include <iostream>
#include <fstream>

#include <business/service/server_service.h>
#include <business/management_server/management_server.h>
#include <business/api/constants.h>
#include <business/game_server/game_server.h>
#include <business/game_server/game_server_config.h>
#include <core/server/http/http_server.h>

using namespace business;
using namespace business::management;
using namespace core;

#define USE_GAME_SERVERS
#define USE_HTTP_SERVER

static std::vector<GameServerConfig> g_gameServerConfigs;
int g_loadedGameServerCount = 0;

void loadGameServerConfigs(std::string hostname, std::vector<GameServerConfig>& gameServerConfigs) {

    gameServerConfigs.push_back({
        {hostname, 9883},
        GameServerEventType::NORMAL, L"Novice", 4, 200,
        { ServerTemplateId::NOVICE, 1.0f, 0, 1000 }
    });

    gameServerConfigs.push_back({
        {hostname, 9884},
        GameServerEventType::NORMAL, L"Advanced", 4, 400,
        { ServerTemplateId::ADVANCED, 1.0f, 1000, 1000000000 }
    });

    gameServerConfigs.push_back({
        {hostname, 9885},
        GameServerEventType::NORMAL, L"Expert", 4, 500,
        { ServerTemplateId::EXPERT, 1.0f, 10000, 1000000000 }
    });

    gameServerConfigs.push_back({
        {hostname, 9886},
        GameServerEventType::NORMAL, L"Professional", 4, 600,
        { ServerTemplateId::PROFESSIONAL, 1.0f, 100000, 1000000000 }
    });

    gameServerConfigs.push_back({
        {hostname, 9887},
        GameServerEventType::NORMAL, L"Free", 4, 100,
        { ServerTemplateId::FREE, 1.0f, 0, 1000000000 }
    });

    gameServerConfigs.push_back({
        {hostname, 9888},
        GameServerEventType::NORMAL, L"Ranking", 4, 800,
        { ServerTemplateId::RANKING, 1.0f, 1000, 1000000000 }
    });

    gameServerConfigs.push_back({
        {hostname, 9889},
        GameServerEventType::NORMAL, L"Training", 4, 0,
        { ServerTemplateId::TRAINING, 0.0f, 0, 0 }
    });

}


void runManagementServer(std::string hostname) {

    for(auto& config : g_gameServerConfigs) {
        printf("Initializing %S game server (%s:%d)... ", config.serverName.data(), config.hostInfo.hostname.data(), config.hostInfo.port);
        business::ServerService::getInstance().startGameServer(config);
        printf("Done.\n", config.serverName);
        ++g_loadedGameServerCount;
    }

    ServerConfig managementServerConfig = { {hostname, 9882} };
    ManagementServer managementServer(managementServerConfig);
    managementServer.run();
}

void runHTTPServer(std::string hostname) {
    HTTPServer httpServer({ hostname, 80 });
    httpServer.run();
}

int main(int argc, char *argv[]) {
    printf("Initializing Open Carom3D Server...\n");

    std::ifstream hostFile;
    std::string hostName;
    hostFile.open("resources/host.txt");
    std::getline(hostFile, hostName);

#ifdef USE_GAME_SERVERS
    loadGameServerConfigs(hostName, g_gameServerConfigs);
    std::thread servers(runManagementServer, hostName);
    servers.detach();

    while(g_loadedGameServerCount < g_gameServerConfigs.size())
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
#endif

#ifdef USE_HTTP_SERVER
    std::thread httpServers(runHTTPServer, hostName);
    httpServers.detach();
#endif

    printf("Open Carom3D Server initialized successfully.\n");

    char command[256];
    std::cin.getline (command,256);
    while(*command) {
        if(::strcmp("quit",command) == 0)
            break;
        std::cin.getline(command, 256);
    }
    return 0;
}
