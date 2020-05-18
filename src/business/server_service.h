//
// Created by CGR on 17/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_SERVER_SERVICE_H__
#define __OPEN_CAROM3D_SERVER_SERVER_SERVICE_H__

#include <list>

class ServerConfig;
class Server;

namespace business {

    class ServerService {
        std::list<Server *> m_servers;

    public:
        static ServerService &getInstance();

        Server *startServer(const ServerConfig &serverConfig);

        std::list<Server *> &getServerList();

        void poll();
    };

}

#endif //__OPEN_CAROM3D_SERVER_SERVER_SERVICE_H__
