//
// Created by CGR on 17/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_SERVER_SERVICE_H__
#define __OPEN_CAROM3D_SERVER_SERVER_SERVICE_H__

#include <list>

namespace core {
    class ServerConfig;
    class Server;
}

namespace business {

    class ServerService {
        std::list<core::Server *> m_servers;

    public:
        static ServerService &getInstance();

        core::Server *startServer(const core::ServerConfig &serverConfig);

        std::list<core::Server *> &getServerList();

        void poll();
    };

}

#endif //__OPEN_CAROM3D_SERVER_SERVER_SERVICE_H__
