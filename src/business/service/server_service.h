//
// Created by CGR on 17/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_SERVER_SERVICE_H__
#define __OPEN_CAROM3D_SERVER_SERVER_SERVICE_H__

#include <list>

#include <business/game_server/game_server.h>

namespace core {
    class Server;
    class ServerConfig;
}

namespace business {
    class GameServer;
    class GameServerConfig;
}

namespace business {
    class core::ServerConfig;
    class core::Server;
    class business::GameServer;
    class business::GameServerConfig;

    class ServerService {
        std::list<core::Server *> m_servers;

    public:
        static ServerService &getInstance();

        core::Server *startGameServer(const business::GameServerConfig &serverConfig);

        std::list<core::Server *> &getServerList();

        void poll();
    };

}

#endif //__OPEN_CAROM3D_SERVER_SERVER_SERVICE_H__
