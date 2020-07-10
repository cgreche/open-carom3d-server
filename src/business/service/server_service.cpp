//
// Created by CGR on 17/05/2020.
//


#include <business/game_server/game_server.h>
#include <business/game_server/game_server_config.h>
#include <thread>
#include "server_service.h"

namespace business {
    using namespace core;

    static ServerService g_serverService;
    static std::list<std::thread*> m_threads;

    //Argument needs to be a pointer (or use std::ref) for correct object pointing
    void runServer(Server* gameServer) {
        for(;;) {
            gameServer->poll();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    ServerService &ServerService::getInstance() {
        return g_serverService;
    }

    core::Server* ServerService::startGameServer(const business::GameServerConfig& serverConfig) {
        GameServer *server = new GameServer(serverConfig);
        auto t = new std::thread(runServer, server);
        t->detach();
        m_threads.emplace_back(t);
        m_servers.emplace_back(server);
        return server;
    }

    void ServerService::poll() {
        for (auto server : m_servers) {
            server->poll();
        }
    }

    std::list<Server *> &ServerService::getServerList() {
        return m_servers;
    }

}