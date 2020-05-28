//
// Created by CGR on 19/05/2020.
//

#include <ctime>
#include <business/util/abstract_action.h>
#include <business/entity/player.h>
#include <business/entity/room.h>
#include <business/game_server/game_server.h>
#include <business/util/action_dispatcher.h>
#include <business/util/destination.h>
#include "room_service.h"

namespace business {

    static RoomService g_roomService;

    //temp: TODO
//    Player createDummyPlayer() {
//        return Player();
//    }


    RoomService &RoomService::getInstance() {
        return g_roomService;
    }

    Room* RoomService::createRoom(User *user, const wchar_t *title, const wchar_t *password, int maxPlayers, const Room::GameInfo& gameInfo) {
        Room *newRoom;
        user->server()->createRoom(title, user, maxPlayers, gameInfo, &newRoom);
        if(nullptr == newRoom)
            return nullptr;

        newRoom->setPassword(password);

        u32 matchType = gameInfo.matchType;
        //TODO(CGR): open slots based in matchType
        switch(matchType) {
            //Normal game
            case 0:
                newRoom->setSlotState(0, Room::SlotState::OPEN);
                newRoom->setSlotState(1, Room::SlotState::OPEN);
                newRoom->setSlotState(2, Room::SlotState::OPEN);
                newRoom->setSlotState(6, Room::SlotState::OPEN);
                newRoom->setSlotState(7, Room::SlotState::OPEN);
                newRoom->setSlotState(8, Room::SlotState::OPEN);
                break;
            case 4:
                newRoom->setSlotState(0, Room::SlotState::OPEN);
                newRoom->setSlotState(6, Room::SlotState::OPEN);
                break;

            //practice
            case 5:
                break;
            default:
                return newRoom;
        }
        //

        int listIndex = newRoom->insertUser(*user);
        user->setSpot(newRoom);

        //TODO(CGR): modularize
        CreatedRoomData roomData = {0};
        ::wcsncpy(roomData.roomTitle, newRoom->title(), ROOM_TITLE_MAX_LEN + 1);
        roomData.roomTitle[ROOM_TITLE_MAX_LEN] = L'\0';
        ::wcsncpy(roomData.roomMaster, newRoom->roomMaster()->player()->name(), PLAYER_NAME_MAX_LEN + 1);
        roomData.roomMaster[PLAYER_NAME_MAX_LEN] = L'\0';
        roomData.unk52 = 2;

        const Room::GameInfo& game = newRoom->gameInfo();
        roomData.roomType = game.roomType;
        roomData.gameType = game.gameType;
        roomData.matchType = game.matchType;
        roomData.difficulty = game.difficulty;

        //room slot infos
        const Room::SlotInfos &slotInfos = newRoom->slotInfos();
        for(int i = 0; i < 30; ++i) {
            roomData.slotInfos.slotState[i] = slotInfos.state[i];
            roomData.slotInfos.playerListIndex[i] = slotInfos.playerListIndex[i];
        }
        ActionData roomCreatedAction(0x25, &roomData, sizeof(roomData));
        user->client().sendAction(roomCreatedAction);

		this->updateSlotInfo(*newRoom);

        RoomPlayerData roomPlayer = {0};
        //notify all players that user joined
        Player *player = user->player();
        ::wcsncpy(roomPlayer.id, player->name(), PLAYER_NAME_MAX_LEN + 1);
        roomPlayer.id[PLAYER_NAME_MAX_LEN] = L'\0';
        roomPlayer.country[0] = L'B';
        roomPlayer.country[1] = L'R';
        roomPlayer.level = player->level();
        roomPlayer.listIndex = listIndex;
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
        ActionData playerInfoActionData(0x27, &roomPlayer, sizeof(roomPlayer));
        RoomPlayerItemData item = {0};
        ActionData playerItemInfosActionData(0x71, &item, sizeof(item));
        ActionDispatcher::prepare()
                .action(playerInfoActionData)
                .action(playerItemInfosActionData)
                .to(UserDestination(*user))
                .send();

        ActionData playerListEnd(0x63, nullptr, 0);
        ActionDispatcher::prepare().action(playerListEnd).to(UserDestination(*user)).send();
        return newRoom;
    }

    void RoomService::insertUserIntoRoom(Room &room, User& user) {
        int listIndex = room.insertUser(user);
        user.setSpot(&room);

        //TODO(CGR): modularize
        JoinedRoomData roomData = {0};
        ::wcsncpy(roomData.roomTitle, room.title(), ROOM_TITLE_MAX_LEN + 1);
        roomData.roomTitle[ROOM_TITLE_MAX_LEN] = L'\0';
        ::wcsncpy(roomData.roomMaster, room.roomMaster()->player()->name(), PLAYER_NAME_MAX_LEN + 1);
        roomData.roomMaster[PLAYER_NAME_MAX_LEN] = L'\0';
        roomData.unk52 = 2;
        roomData.state = room.state();

        const Room::GameInfo& game = room.gameInfo();
        roomData.roomType = game.roomType;
        roomData.gameType = game.gameType;
        roomData.matchType = game.matchType;
        roomData.difficulty = game.difficulty;

        //room slot infos
        const Room::SlotInfos &slotInfos = room.slotInfos();
        for(int i = 0; i < 30; ++i) {
            roomData.slotInfos.slotState[i] = slotInfos.state[i];
            roomData.slotInfos.playerListIndex[i] = slotInfos.playerListIndex[i];
        }
        ActionData roomCreatedAction(0x24, &roomData, sizeof(roomData));
        user.client().sendAction(roomCreatedAction);

        //get all players in room
        for(auto userListIndex : room.userListIndexes()) {
            User *userIn = room.userInListIndex(userListIndex);
            if(userIn == &user)
                continue;
            Player *player = userIn->player();

            RoomPlayerData roomPlayer = {0};
            ::wcsncpy(roomPlayer.id, player->name(), PLAYER_NAME_MAX_LEN);
            roomPlayer.id[PLAYER_NAME_MAX_LEN] = L'\0';
            roomPlayer.country[0] = L'B';
            roomPlayer.country[1] = L'R';
            roomPlayer.level = player->level();
            roomPlayer.listIndex = userListIndex;
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
            roomPlayer.unk6D = 15;
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

            ActionData playerInfoActionData(0x27, &roomPlayer, sizeof(roomPlayer));
            RoomPlayerItemData item = {0};
            ActionData playerItemInfosActionData(0x71, &item, sizeof(item));

            ActionDispatcher::prepare().action(playerInfoActionData).to(UserDestination(user)).send();
            ActionDispatcher::prepare().action(playerItemInfosActionData).to(UserDestination(user)).send();
        }

        RoomPlayerData roomPlayer = {0};
        //notify all players that user joined
        Player *player = user.player();
        ::wcsncpy(roomPlayer.id, player->name(), PLAYER_NAME_MAX_LEN + 1);
        roomPlayer.id[PLAYER_NAME_MAX_LEN] = L'\0';
        roomPlayer.country[0] = L'B';
        roomPlayer.country[1] = L'R';
        roomPlayer.level = player->level();
        roomPlayer.listIndex = listIndex;
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
        ActionData playerInfoActionData(0x27, &roomPlayer, sizeof(roomPlayer));
        RoomPlayerItemData item = {0};
        ActionData playerItemInfosActionData(0x71, &item, sizeof(item));
        ActionDispatcher::prepare()
                            .action(playerInfoActionData)
                            .action(playerItemInfosActionData)
                            .to(RoomDestination(room))
                            .send();

        ActionData playerListEnd(0x63, nullptr, 0);
        ActionDispatcher::prepare().action(playerListEnd).to(UserDestination(user)).send();
    }

    Room* RoomService::getRoom(GameServer& server, const wchar_t* roomTitle) {
        return server.getRoom(roomTitle);
    }

    void RoomService::removeUserFromRoom(Room& room, User& user) {
        int listIndex = room.getUserListIndex(user);
        if(listIndex >= 0) {
            removeUserFromRoom(room, listIndex);
            user.setSpot(nullptr);
        }

	}

    void RoomService::removeUserFromRoom(Room &room, int userListIndex) {
        User* user = room.userInListIndex(userListIndex);
        if(!user)
            return;
		
		User* currentRoomMaster = room.roomMaster();
        room.removeUser(userListIndex);
		User* newRoomMaster = room.roomMaster();

		ActionData exitRoomAction(0x26);
		ActionDispatcher::prepare().action(exitRoomAction).to(UserDestination(*user)).send();

		if (room.usersInCount() != 0) {
			if (currentRoomMaster != newRoomMaster) {
				ActionData changeRoomMasterAction(0x32, newRoomMaster->player()->name(), (PLAYER_NAME_MAX_LEN + 1)*2);
				ActionDispatcher::prepare().action(changeRoomMasterAction).to(RoomDestination(room)).send();
				notifyChannelOfRoomMasterChange(room.server(), room);
			}

			updateSlotInfo(room);
			int listIndex = userListIndex;
			ActionData userExitedRoomAction(0x28, &listIndex, 4);
			ActionDispatcher::prepare().action(userExitedRoomAction).to(RoomDestination(room)).send();
		}
		else {
			int roomId = room.id();

			GameServer& server = room.server();
			server.destroyRoom(room);

			ActionData playerExitedRoomAction(0x2D, &roomId, 4);
			ActionDispatcher::prepare().action(playerExitedRoomAction).to(ServerDestination(server, 1)).send();
		}
    }

    void RoomService::setUserToSlot(Room& room, int slot, User* user) {
        int result;
        if(nullptr == user)
            result = room.freeSlot(slot);
        else {
            if(user == room.roomMaster())
                return;
            result = room.setUserToSlot(slot, *user);
        }

        //TODO(CGR): handler errors better
        if(result == 1)
            return;

        //result == 0 means no error, update everyone in the room

        //TODO(CGR): modularize
        int listIndex = room.getSlotUserListIndex(slot);
        SlotModificationResultData modificationResultData = {listIndex, slot, 0};
        for(auto userIndex : room.userListIndexes()) {
            User* userIn = room.userInListIndex(userIndex);
            ActionData playerJoinSlotAction(0x4D, (u8 *) &modificationResultData, sizeof(modificationResultData));
            userIn->client().sendAction(playerJoinSlotAction);
        }
    }

    void RoomService::setSlotState(Room& room, int slot, Room::SlotState state) {
        room.setSlotState(slot, state);

        //TODO(CGR): modularize
        //TODO(CGR): set to non-literal constant
        int listIndex = room.getSlotUserListIndex(slot);
        int slotState = room.getSlotState(slot);
        SlotModificationResultData modificationResultData = {listIndex, slot, (int)slotState};
        ActionData slotStateActionData(0x4D, (u8 *) &modificationResultData, sizeof(modificationResultData));
        ActionDispatcher::prepare().action(slotStateActionData).to(RoomDestination(room)).send();
    }

	void RoomService::updateSlotInfo(Room& room) {
		RoomSlotInfoData slotInfoData;
		const Room::SlotInfos& slotInfos = room.slotInfos();
		for (int i = 0; i < 30; ++i) {
			slotInfoData.slotState[i] = slotInfos.state[i];
			slotInfoData.playerListIndex[i] = slotInfos.playerListIndex[i];
		}
		ActionData slotInfoActionData(0x51, &slotInfoData, sizeof(slotInfoData));
		ActionDispatcher::prepare().action(slotInfoActionData).to(RoomDestination(room)).send();
	}

    void RoomService::changeRoomState(Room &room, bool open) {
        room.setClosed(!open);
        //TODO(CGR): updateRoom
    }

    void RoomService::sendMessageToRoom(Room &room, User &sender, const wchar_t *message) {
        int userListIndex = room.getUserListIndex(sender);
        if(userListIndex >= 0) {
#pragma pack(push, 1)
            struct RoomUserMessage {
                int userListIndex;
                wchar_t message[8192];
            };
#pragma pack(pop)
            RoomUserMessage roomUserMessageData;
            roomUserMessageData.userListIndex = userListIndex;
            ::wcscpy(roomUserMessageData.message, message);
            ActionData roomUserMessageActionData(0x55, &roomUserMessageData, 4+(::wcslen(message)+1)*2);
            ActionDispatcher::prepare().action(roomUserMessageActionData).to(RoomDestination(room)).send();
        }
    }

    void RoomService::startGame(Room* room) {
        room->startGame();

        std::srand(std::time(nullptr)); // use current time as seed for random generator
        int startRandomSeed = std::rand();
        ActionData matchStartedActionData(0x33, (u8 *) &startRandomSeed, 4);
        ActionDispatcher::prepare().action(matchStartedActionData).to(RoomDestination(*room)).send();
    }

	void RoomService::notifyChannelOfRoomMasterChange(const GameServer& server, const Room& room) {
		struct RoomUpdateInfo {
			u32 roomId;
			wchar_t roomMaster[PLAYER_NAME_MAX_LEN + 1];
		} updateInfo;
		User* roomMaster = room.roomMaster();
		updateInfo.roomId = room.id();
		::wcsncpy(updateInfo.roomMaster, roomMaster ? roomMaster->player()->name() : L"", PLAYER_NAME_MAX_LEN);
		updateInfo.roomMaster[PLAYER_NAME_MAX_LEN] = L'\0';

		ActionData action(0x2E, &updateInfo, sizeof(updateInfo));
		ActionDispatcher::prepare().action(action).to(ServerDestination(server, 1)).send();
	}


}