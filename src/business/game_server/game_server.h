//
// Created by CGR on 15/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_GAME_SERVER_H__
#define __OPEN_CAROM3D_SERVER_GAME_SERVER_H__

#include <list>
#include <core/server/carom3d/carom3d_server.h>
#include <business/entity/room.h>
#include "game_server_config.h"

namespace business {

    class User;
    class Room;
    using core::ActionData;

    class GameServer : public core::Carom3DServer {
        std::wstring m_qualifiedName;
        std::list<Room*> m_rooms;

    public:
        explicit GameServer(const GameServerConfig &config);
       
        void onClientDisconnection(core::ClientSession& client) override;

        core::MessagingProtocol* messagingProtocol() override;

        int createRoom(const wchar_t* title, User* user, int maxPlayers, const Room::GameInfo& gameInfo, Room** pRetRoom);
        Room* getRoom(const wchar_t* title);
        int destroyRoom(Room& room);

        const std::wstring& qualifiedName() const { return m_qualifiedName; }
        const std::list<Room*>& rooms() const { return m_rooms; }


        GameServerConfig gsConfig;
    };

}

#endif //__OPEN_CAROM3D_SERVER_GAME_SERVER_H__
