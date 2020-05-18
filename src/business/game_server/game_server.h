//
// Created by CGR on 15/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_GAME_SERVER_H__
#define __OPEN_CAROM3D_SERVER_GAME_SERVER_H__

#include <core/server/server_context.h>

namespace game {

    class GameServer : public Server {
    public:
        explicit GameServer(const ServerConfig &config);
        void onClientConnection(ClientSession *client) override;
        void onClientDisconnection(ClientSession *client) override;
    };

}

#endif //__OPEN_CAROM3D_SERVER_GAME_SERVER_H__
