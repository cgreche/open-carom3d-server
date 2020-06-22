//
// Created by CGR on 13/05/2020.
//

#include <thread>
#include <iostream>
#include <fstream>

#include <business/service/server_service.h>
#include <business/management_server/management_server.h>
#include <business/game_server/game_server.h>
#include <core/server/http/http_server.h>

using namespace business::management;
using namespace core;

void runManagementServer(std::string hostname) {
    ServerConfig managementServerConfig = { hostname, 9882 };

    ServerConfig noviceSvConfig = { hostname, 9883 };
    ServerConfig advancedSvConfig = { hostname, 9884 };
    ServerConfig expertSvConfig = { hostname, 9885 };
    ServerConfig proSvConfig = { hostname, 9886 };
    ServerConfig trainingSvConfig = { hostname, 9887 };
    ServerConfig freeSvConfig = { hostname, 9888 };

    ManagementServer managementServer(managementServerConfig);
    business::ServerService::getInstance().startServer(noviceSvConfig);
    business::ServerService::getInstance().startServer(advancedSvConfig);
    business::ServerService::getInstance().startServer(expertSvConfig);
    business::ServerService::getInstance().startServer(proSvConfig);
    business::ServerService::getInstance().startServer(trainingSvConfig);
    business::ServerService::getInstance().startServer(freeSvConfig);

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

    std::thread servers(runManagementServer, hostName);
    servers.detach();

    std::thread httpServers(runHTTPServer, hostName);
    httpServers.detach();

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
