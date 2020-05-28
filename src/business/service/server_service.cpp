//
// Created by CGR on 17/05/2020.
//

#include "server_service.h"
#include <business/game_server/game_server.h>
#include <thread>

namespace business {
    using namespace core;

    static ServerService g_serverService;
    static std::list<std::thread*> m_threads;

    void runServer(Server& gameServer) {
        for(;;)
            gameServer.poll();
    }

    ServerService &ServerService::getInstance() {
        return g_serverService;
    }

    Server *ServerService::startServer(const ServerConfig &serverConfig) {
        Server *server = new GameServer(serverConfig);
        auto t = new std::thread(runServer, *server);
        t->detach();
        m_threads.emplace_back(t);
        m_servers.emplace_back(server);
        return nullptr;
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