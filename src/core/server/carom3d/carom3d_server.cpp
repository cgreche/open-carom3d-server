//
// Created by CGR on 09/06/2020.
//

#include "carom3d_protocol.h"
#include "carom3d_server.h"

namespace core {

    Carom3DServer::Carom3DServer(const ServerConfig &config)
        : Server(config) {
    }

    MessagingProtocol* Carom3DServer::messagingProtocol() {
        return new Carom3DProtocol;
    }

}