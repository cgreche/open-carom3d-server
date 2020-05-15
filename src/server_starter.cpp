//
// Created by CGR on 13/05/2020.
//

#include "core/server/messaging/message_listener.h"
#include "core/server/server_config.h"
#include "core/server/server_context.h"

int main(int argc, char *argv[]) {

    printf("Initializing...\n");

    ServerConfig config;
    config.port = 9882;
    Server server(config);
    extern std::map<int, Action*> actions;
    server.setActionMap(&actions);
    server.run();
    return 0;
}
