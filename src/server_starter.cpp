//
// Created by CGR on 13/05/2020.
//

#include <business/service/server_service.h>
#include <business/management_server/management_server.h>
#include <business/game_server/game_server.h>

#include <thread>
#include <iostream>

using namespace business::management;
using namespace core;

//Change it to your Internet IP/Host
#define HOST "192.168.15.10"

void runManagementServer() {
    ServerConfig managementServerConfig = {HOST, 9882};
    ServerConfig gameServer1Config = {HOST, 9883};

    ManagementServer managementServer(managementServerConfig);
    business::ServerService::getInstance().startServer(gameServer1Config);

    managementServer.run();
}

int main(int argc, char *argv[]) {
    printf("Initializing Open Carom3D Server...\n");

    std::thread servers(runManagementServer);
    servers.detach();

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
