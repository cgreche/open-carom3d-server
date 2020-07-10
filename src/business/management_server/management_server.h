//
// Created by CGR on 15/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_MANAGEMENT_SERVER_H__
#define __OPEN_CAROM3D_SERVER_MANAGEMENT_SERVER_H__

#include <core/server/carom3d/carom3d_server.h>

#include <list>

namespace business { namespace management {

    class ManagementServer : public core::Carom3DServer {
    public:
        explicit ManagementServer(const core::ServerConfig &config);
        
        core::MessagingProtocol* messagingProtocol() override;

        void onClientConnection(core::ClientSession& client) override;
        void onClientDisconnection(core::ClientSession& client) override;
    };

}}

#endif //__OPEN_CAROM3D_SERVER_MANAGEMENT_SERVER_H__
