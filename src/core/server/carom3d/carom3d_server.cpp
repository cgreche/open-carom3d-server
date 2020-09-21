//
// Created by CGR on 09/06/2020.
//

#include <ctime>
#include "carom3d_protocol.h"
#include "carom3d_server.h"
#include "carom3d_user_session.h"

namespace core {

    Carom3DServer::Carom3DServer(const ServerConfig &config)
        : Server(config) {
    }

    MessagingProtocol* Carom3DServer::messagingProtocol() {
        return new Carom3DProtocol;
    }

    void Carom3DServer::poll() {
        Server::poll();

        time_t now = ::time(nullptr);
        for(std::pair<unsigned long, ClientSession*> client : clients()) {
            Carom3DUserSession* user = ((Carom3DUserSession*)client.second);
            if(now - user->lastMessageTime() >= 60) {
                disconnectClient(user);
            }
        }
    }

}