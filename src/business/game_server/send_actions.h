//
// Created by CGR on 20/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_SEND_ACTIONS_H__
#define __OPEN_CAROM3D_SERVER_SEND_ACTIONS_H__

#include "game_server_action.h"
#include <core/server/carom3d/util/action_builder.h>

namespace business {

    static void fillRoomInfo(RoomInfoActionData& roomInfo, const Room& room) {
        const Room::GameInfo& game = room.gameInfo();
        roomInfo.roomId = room.id();
        roomInfo.difficulty = game.difficulty;
        ::wcscpy(roomInfo.roomName, room.name());
        roomInfo.playersIn = room.usersInCount();
        roomInfo.maxPlayers = room.maxPlayers();
        roomInfo.u = 3;
        roomInfo.levelLimit = 0;
        roomInfo.gameType = game.gameType;
        roomInfo.roomType = game.roomType;
        roomInfo.matchType = game.matchType;
        roomInfo.roomState = room.inGame();
        ::wcscpy(roomInfo.roomMaster, room.roomMaster() ? room.roomMaster()->player()->name() : L"");
        roomInfo.straightWins = room.straightWins();
        roomInfo.caneys = game.caneys;
    }

    class RoomJoinActionTemplate : public ActionTemplate {
        const Room& m_room;

    public:
        RoomJoinActionTemplate(const Room& room)
            : m_room(room) {}

        const ActionData data() override {
            const Room& room = m_room;
            User* roomMaster = room.roomMaster();

            JoinedRoomData roomData = { 0 };
            ::wcscpy(roomData.roomTitle, room.name());
            ::wcscpy(roomData.roomMaster, roomMaster == nullptr ? L"" : roomMaster->player()->name());
            roomData.unk52 = 2;
            roomData.state = room.state();

            const Room::GameInfo& game = room.gameInfo();
            roomData.roomType = game.roomType;
            roomData.gameType = game.gameType;
            roomData.matchType = game.matchType;
            roomData.difficulty = game.difficulty;

            //TODO(CGR): update slots to everyone in the room if match type == CHALLENGE
            //room slot infos
            const Room::SlotInfos& slotInfos = room.slotInfos();
            for(int i = 0; i < 30; ++i) {
                roomData.slotInfos.slotState[i] = slotInfos.state[i];
                roomData.slotInfos.playerListIndex[i] = slotInfos.playerListIndex[i];
            }
            return ActionData(0x24, &roomData, sizeof(roomData));
        }
    };

    class RoomCreationActionTemplate : public ActionTemplate {
        const Room& m_room;

    public:
        RoomCreationActionTemplate(const Room& room)
            : m_room(room) {}

        const ActionData data() override {
            const Room& room = m_room;
            User* roomMaster = room.roomMaster();

            CreatedRoomData roomData = { 0 };
            ::wcscpy(roomData.roomTitle, room.name());
            ::wcscpy(roomData.roomMaster, roomMaster == nullptr ? L"" : roomMaster->player()->name());
            roomData.unk52 = 2;

            const Room::GameInfo& game = room.gameInfo();
            roomData.roomType = game.roomType;
            roomData.gameType = game.gameType;
            roomData.matchType = game.matchType;
            roomData.difficulty = game.difficulty;

            //room slot infos
            const Room::SlotInfos& slotInfos = room.slotInfos();
            for(int i = 0; i < 30; ++i) {
                roomData.slotInfos.slotState[i] = slotInfos.state[i];
                roomData.slotInfos.playerListIndex[i] = slotInfos.playerListIndex[i];
            }

            return ActionData(0x25, &roomData, sizeof(roomData));
        }

    };

    class RoomPlayerJoinActionTemplate : public ActionTemplate {
        const Player& m_player;
        int m_listIndex;

    public:
        explicit RoomPlayerJoinActionTemplate(const Player& player, int listIndex)
            : m_player(player), m_listIndex(listIndex) { }

        const ActionData data() override {
            const Player& player = m_player;

            RoomPlayerData roomPlayer = { 0 };
            ::wcscpy(roomPlayer.id, player.name());
            roomPlayer.id[PLAYER_NAME_MAX_LEN] = L'\0';
            ::wcscpy(roomPlayer.country, L"BR");
            roomPlayer.level = player.level();
            roomPlayer.listIndex = m_listIndex;
            roomPlayer.cueId = 3000;
            roomPlayer.unk130 = 1;
            roomPlayer.power = 150;
            roomPlayer.power_range = 150;
            roomPlayer.chalks = 0;
            roomPlayer.control = 150;
            roomPlayer.backSpin = 54;
            roomPlayer.topSpin = 54;
            roomPlayer.sideSpin = 54;
            roomPlayer.unk84 = 0;
            roomPlayer.unk6D = 0;
            roomPlayer.unk120 = 0;
            roomPlayer.unk121 = 0;
            roomPlayer.unk130 = 0;
            roomPlayer.unk145 = 0;
            roomPlayer.charGender = 1;
            roomPlayer.unused99 = 0;
            roomPlayer.unk6D = 0;
            roomPlayer.unusedA1 = 0;
            roomPlayer.unkA5 = 0;
            roomPlayer.unusedA9 = 0;
            roomPlayer.unkA5 = 0;
            return ActionData(0x27, &roomPlayer, sizeof(roomPlayer));
        }
    };

    class PlayerProfileActionTemplate : public ActionTemplate {
        Player& m_player;

    public:
        explicit PlayerProfileActionTemplate(Player& player)
            : m_player(player) {
        }

        const ActionData data() override {
            PlayerProfileData data = { 0 };
            data.playerNumber = m_player.id();
            ::wcscpy(data.playerName, m_player.name());
            ::wcscpy(data.playerGuild, L"");
            ::wcscpy(data.loggedServerName, L""); //TODO(CGR): get server name
            data.serverId = 0; //TODO(CGR): get server id
            ::wcscpy(data.country, L"BR"); //TODO(CGR): ip_to_country
            data.wins = 0 * 10;
            data.level = m_player.level();
            return ActionData(0x3A, &data, sizeof(data));
        }
    };

    class UserPrivateMessageActionTemplate : public ActionTemplate {
        const wchar_t* m_userName;
        const wchar_t* m_message;

    public:
        explicit UserPrivateMessageActionTemplate(const wchar_t* userName, const wchar_t* message)
            : m_userName(userName), m_message(message) {
        }

        const ActionData data() override {
            return ActionBuilder(0x3B)
                .add(m_userName, (PLAYER_NAME_MAX_LEN + 1) * 2)
                .add(m_message)
                .build();
        }
    };

    class UserInviteActionTemplate : public ActionTemplate {
        User& m_user;
        Room& m_room;

    public:
        explicit UserInviteActionTemplate(User& user, Room& room)
            : m_user(user), m_room(room) {
        }

        const ActionData data() override {
            UserInvite userInvite;
            ::wcscpy(userInvite.playerName, m_user.player()->name());
            fillRoomInfo(userInvite.roomInfo, m_room);
            ::wcscpy(userInvite.roomPassword, m_room.password());
            return ActionData(0x4B, &userInvite, sizeof(userInvite));
        }
    };

    class ExistingRoomsNotificationActionTemplate : public ActionTemplate {
        const Room& m_room;

    public:
        ExistingRoomsNotificationActionTemplate(const Room& room)
            : m_room(room) {}

        const ActionData data() override {
            RoomInfoActionData actionData;
            fillRoomInfo(actionData, m_room);
            return ActionData(0x2B, &actionData, sizeof(actionData));
        }
    };

    class RoomCreationNotifyActionTemplate : public ActionTemplate {
        const Room& m_room;

    public:
        RoomCreationNotifyActionTemplate(const Room& room)
            : m_room(room) {}

        const ActionData data() override {
            RoomInfoActionData actionData;
            fillRoomInfo(actionData, m_room);
            return ActionData(0x2A, &actionData, sizeof(actionData));
        }
    };

}

#endif //__OPEN_CAROM3D_SERVER_ACTIONS_H__
