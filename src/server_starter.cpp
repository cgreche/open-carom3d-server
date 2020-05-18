//
// Created by CGR on 13/05/2020.
//

#include <business/server_service.h>
#include <business/management_server/management_server.h>
#include <business/game_server/game_server.h>

#include <thread>
#include <iostream>

void runManagementServer() {
    //Change it to your Internet IP
    ServerConfig managementServerConfig = {"127.0.0.1", 9882};
    ServerConfig gameServer1Config = {"127.0.0.1", 9883};

    management::ManagementServer managementServer(managementServerConfig);
    business::ServerService::getInstance().startServer(gameServer1Config);

    managementServer.run();
}

int main(int argc, char *argv[]) {
    printf("Initializing...\n");
    std::thread servers(runManagementServer);
    servers.detach();

    char command[256];
    std::cin.getline (command,256);
    while(*command) {
        if(::strcmp("quit",command) == 0)
            break;
        std::cin.getline(command, 256);
    }
    return 0;
}
