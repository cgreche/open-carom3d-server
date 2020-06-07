//
// Created by CGR on 15/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_GAME_SERVER_H__
#define __OPEN_CAROM3D_SERVER_GAME_SERVER_H__

#include <list>
#include <core/server/server_config.h>
#include <core/server/server_context.h>
#include <business/entity/room.h>

namespace business {

    class User;
    class Room;
    using core::ActionData;

    class GameServer : public core::Server {
        std::list<User*> m_users;
        std::list<Room*> m_rooms;

    public:
        explicit GameServer(const core::ServerConfig &config);
       
        void onClientConnection(core::ClientSession *client) override;
        void onUnhandledClientAction(core::ClientSession* client, const ActionData& actionData) override;
        void onClientDisconnection(core::ClientSession *client) override;

        int createRoom(const wchar_t* title, User* user, int maxPlayers, const Room::GameInfo& gameInfo, Room** pRetRoom);
        Room* getRoom(const wchar_t* title);
        int destroyRoom(Room& room);

        const std::list<User*>& users() const { return m_users; }
        const std::list<Room*>& rooms() const { return m_rooms; }
    };

}

#endif //__OPEN_CAROM3D_SERVER_GAME_SERVER_H__
