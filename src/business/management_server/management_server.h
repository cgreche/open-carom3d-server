//
// Created by CGR on 15/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_MANAGEMENT_SERVER_H__
#define __OPEN_CAROM3D_SERVER_MANAGEMENT_SERVER_H__

#include <core/server/server_context.h>

#include <list>

namespace management {

    class ManagementServer : public Server {
    public:
        explicit ManagementServer(const ServerConfig &config);

        void onClientConnection(ClientSession *client) override;
        void run() override;
    };

}

#endif //__OPEN_CAROM3D_SERVER_MANAGEMENT_SERVER_H__
