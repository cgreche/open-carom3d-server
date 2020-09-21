//
// Created by CGR on 09/06/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_CAROM3D_SERVER_H__
#define __OPEN_CAROM3D_SERVER_CAROM3D_SERVER_H__

#include <map>
#include <core/server/server_context.h>

namespace core {

    class ClientSession;
    class Action;

    class Carom3DServer : public Server {
    public:
        explicit Carom3DServer(const ServerConfig &config);
        virtual ~Carom3DServer() {};

        MessagingProtocol* messagingProtocol() override;

        void poll() override;
    };

}

#endif //__OPEN_CAROM3D_SERVER_CAROM3D_SERVER_H__
