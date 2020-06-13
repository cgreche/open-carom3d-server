//
// Created by CGR on 15/05/2020.
//

#include <map>
#include <business/entity/account.h>
#include <business/entity/room.h>
#include <business/service/channel_service.h>
#include <business/service/user_service.h>
#include <business/service/room_service.h>
#include <core/server/carom3d/carom3d_protocol.h>
#include "game_server.h"
#include "actions.h"

namespace business {

#define LOGIN_ACTION 0x01
#define JOIN_CHANNEL_ACTION 0x03
#define CHANNEL_MESSAGE_ACTION 0x04
#define CREATE_ROOM_ACTION 0x05
#define JOIN_ROOM_ACTION 0x06
#define EXIT_ROOM_ACTION 0x07
#define EXIT_MATCH_ACTION 0x08
#define START_MATCH_ACTION 0x09
#define END_MATCH_ACTION 0x0A
#define PLAYER_PROFILE_REQUEST 0x35
#define PERSONAL_MESSAGE_ACTION 0x36 //TODO(CGR)
#define MATCH_EVENT_INFO_ACTION 0x69
#define MATCH_EVENT_INFO_ACTION2 0x6B
#define ROOM_SLOT_MODIFICATION_ACTION 0x6D
#define ROOM_KICK_PLAYER_ACTION 0x96
#define MATCH_MAKER_SCREEN_REQUEST 0x97
#define ROOM_MESSAGE_ACTION 0x9A

//Action Sent to player: 0x4F (LG_BANUSER): TODO(CGR): what is this?

    static std::map<int, Action *> actions = {
            { LOGIN_ACTION, new LoginAction },
            { JOIN_CHANNEL_ACTION, new JoinChannelAction },
            { CREATE_ROOM_ACTION, new CreateRoomAction},
            { JOIN_ROOM_ACTION, new JoinRoomAction },
            { CHANNEL_MESSAGE_ACTION, new ChannelMessageAction},
            { EXIT_ROOM_ACTION, new ExitRoomAction },
            { EXIT_MATCH_ACTION, new ExitRoomAction },
			{ START_MATCH_ACTION, new StartMatchAction },
			{ END_MATCH_ACTION, new EndMatchAction },
			{ PLAYER_PROFILE_REQUEST, new PlayerProfileRequestAction },
            { MATCH_EVENT_INFO_ACTION, new MatchEventInfoAction },
            { MATCH_EVENT_INFO_ACTION2, new MatchEventInfoAction2 },
            { ROOM_SLOT_MODIFICATION_ACTION, new RoomSlotModificationAction },
            { ROOM_KICK_PLAYER_ACTION, new RoomKickPlayerAction },
            { MATCH_MAKER_SCREEN_REQUEST, new MatchMakerScreenRequestAction },
            { ROOM_MESSAGE_ACTION, new RoomMessageAction },
    };


    class GameServerProtocol : public core::Carom3DProtocol {
    public:
        GameServerProtocol() {
            this->setUserActionMap(&actions);
        }

        core::ClientSession* createSession(nettools::ntConnection& ntClient, core::Server& server) {
            return new User(ntClient, server);
        }

        void onUnhandledUserAction(core::Carom3DUserSession& session, const ActionData& actionData) {
            //TODO: Invalid Action
            User& user = (User&)session;
            if(!user.player())
                core::Carom3DProtocol::onUnhandledUserAction(session, actionData);
            else {
                printf("Unhandled client action: %S - %x - %d\n",
                    user.player()->name(),
                    actionData.id(),
                    actionData.data().size());
                printf("Data: \n");
                for(u8 byte : actionData.data()) {
                    printf("%x ", byte);
                }
                printf("\n");
                for(u8 byte : actionData.data()) {
                    printf("%c ", byte <= 0x7F ? byte : ' ');
                }
                printf("\n\n");
            }
        }
    };

    static GameServerProtocol g_protocol;

    GameServer::GameServer(const core::ServerConfig &config)
            : core::Carom3DServer(config) {
    }

    core::MessagingProtocol* GameServer::messagingProtocol() {
        return &g_protocol;
    }

    void GameServer::onClientDisconnection(core::ClientSession& client) {
        User& user = (User&)client;
        UserSpot* spot = user.spot();
        if(spot) {
            if(spot->isOfType(0))
                ChannelService::getInstance().removeUserFromChannel(*(Channel*)spot, user);
            if(spot->isOfType(1))
                RoomService::getInstance().removeUserFromRoom(*(Room*)spot, user);
        }
        UserService::getInstance().logoutUser(user);
        this->Carom3DServer::onClientDisconnection(client);
    }

    int GameServer::createRoom(const wchar_t *title, User *user, int maxPlayers, const Room::GameInfo &gameInfo, Room **pRetRoom) {
        static int roomId = 0;
        Room *room = new Room(*this, roomId++, title, user, maxPlayers, gameInfo);
        *pRetRoom = room;
        m_rooms.push_back(room);
        return 0;
    }

    Room *GameServer::getRoom(const wchar_t *title) {
        //TODO(CGR): more efficient way to find room
        for(auto room : m_rooms) {
            if(::wcscmp(room->title(),title) == 0)
                return room;
        }
        return nullptr;
    }

    int GameServer::destroyRoom(Room& room) {
		//TODO(CGR): better way to remove room
		for (auto it = m_rooms.begin(); it != m_rooms.end(); ++it)
			if (*it == &room) {
				m_rooms.erase(it);
				return 0;
			}
		return 1;
    }

}