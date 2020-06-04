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
#include "channel_service.h"
#include "room_service.h"

namespace business {

#define ROOM_MASTER_SLOT 0

    static RoomService g_roomService;

    extern const int NORMAL_GAME_SLOT_LAYOUT[ROOM_SLOT_COUNT];
    extern const int DEATH_MATCH_SLOT_LAYOUT[ROOM_SLOT_COUNT];
    extern const int CARD_BALL_SLOT_LAYOUT[ROOM_SLOT_COUNT];
    extern const int CHALLENGE_ROOM_SLOT_LAYOUT[ROOM_SLOT_COUNT];

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

        const int* slotStatesLayout = nullptr;

        //TODO(CGR): create a MATCH_TYPE enum
        //TODO(CGR): create a GAME_TYPE enum
        int matchType = newRoom->gameInfo().matchType;
        int gameType = newRoom->gameInfo().gameType;
        switch(matchType) {
            //Normal game
        case 0:
            //TODO(CGR): DeatchMatch and CardBall games
            //if(gameType ==)
            //slotStatesLayout = DeathMatchSlotLayout;
            //slotStatesLayout = CardBallSlotLayout;
            slotStatesLayout = NORMAL_GAME_SLOT_LAYOUT;
            break;
        case 4:
            slotStatesLayout = CHALLENGE_ROOM_SLOT_LAYOUT;
            break;

            //practice
        case 5:
            //TODO(CGR):
            break;
        default:
            break;
        }
        newRoom->setSlotStates((Room::SlotState*)slotStatesLayout);

        int listIndex = newRoom->insertUser(*user);
        user->setSpot(newRoom);
        newRoom->setUserToSlot(listIndex, ROOM_MASTER_SLOT);

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

        //TODO(CGR): room creation fail

        this->resetRoom(*newRoom);

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

        notifyServerOfRoomCreation(newRoom->server(), *newRoom);
        return newRoom;
    }

    void RoomService::insertUserIntoRoom(Room &room, User& user) {
        int listIndex = room.insertUser(user);
        user.setSpot(&room);

        updateRoom(room);

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

        //TODO(CGR): update slots to everyone in the room if match type == CHALLENGE
        //room slot infos
        const Room::SlotInfos &slotInfos = room.slotInfos();
        for(int i = 0; i < 30; ++i) {
            roomData.slotInfos.slotState[i] = slotInfos.state[i];
            roomData.slotInfos.playerListIndex[i] = slotInfos.playerListIndex[i];
        }
        ActionData roomCreatedAction(0x24, &roomData, sizeof(roomData));
        user.client().sendAction(roomCreatedAction);

        //get all players in room
        for(auto userListIndex : room.userQueue()) {
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

        //TODO(CGR): process fail result
        notifyServerOfRoomPlayerCountUpdate(*user.server(), room);
    }

    Room* RoomService::getRoom(GameServer& server, const wchar_t* roomTitle) {
        return server.getRoom(roomTitle);
    }

    void RoomService::removeUserFromRoom(Room& room, User& user) {
        int listIndex = room.getUserListIndex(user);
        if(listIndex >= 0) {
            removeUserFromRoom(room, listIndex);
        }
	}

    void RoomService::removeUserFromRoom(Room &room, int userListIndex) {
        User* user = room.userInListIndex(userListIndex);
        if(!user)
            return;
		
		User* currentRoomMaster = room.roomMaster();
        room.removeUser(userListIndex);
        user->setSpot(nullptr);
        User* newRoomMaster = room.roomMaster();
        updateRoom(room);

		ActionData exitRoomAction(0x26);
		ActionDispatcher::prepare().action(exitRoomAction).to(UserDestination(*user)).send();

		if (room.usersInCount() != 0) {
			if (currentRoomMaster != newRoomMaster) {
                if(!room.inGame())
                    updateSlotInfo(room);

				ActionData changeRoomMasterAction(0x32, newRoomMaster->player()->name(), (PLAYER_NAME_MAX_LEN + 1)*2);
				ActionDispatcher::prepare().action(changeRoomMasterAction).to(RoomDestination(room)).send();
				notifyServerOfRoomMasterChange(room.server(), room);
			}

			int listIndex = userListIndex;
			ActionData userExitedRoomAction(0x28, &listIndex, 4);
			ActionDispatcher::prepare().action(userExitedRoomAction).to(RoomDestination(room)).send();
		}
		else {
			int roomId = room.id();

			GameServer& server = room.server();
			server.destroyRoom(room);

			ActionData destroyRoomAction(0x2D, &roomId, 4);
			ActionDispatcher::prepare().action(destroyRoomAction).to(ServerDestination(server, 1)).send();
		}

        Channel* lastChannel = ChannelService::getInstance().getChannel(user->lastChannelName());
        if(nullptr != lastChannel) {
            ChannelService::getInstance().insertUserIntoChannel(*lastChannel, *user);
            UserService::getInstance().updateUserWithAllServerRooms(*user);
        }
    }

    void RoomService::setUserToSlot(Room& room, int slot, User* user) {
        int result;
        if(nullptr == user)
            result = room.freeSlot(slot);
        else {
            if(user == room.roomMaster())
                return;
            result = room.setUserToSlot(*user, slot);
        }

        //TODO(CGR): handler errors better
        if(result == 1)
            return;

        //result == 0 means no error, update everyone in the room

        //TODO(CGR): modularize
        int listIndex = room.getSlotUserListIndex(slot);
        SlotModificationResultData modificationResultData = {listIndex, slot, 0};
        for(auto userIndex : room.userQueue()) {
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
                wchar_t message[81];
            };
#pragma pack(pop)
            RoomUserMessage roomUserMessageData;
            roomUserMessageData.userListIndex = userListIndex;
            ::wcscpy(roomUserMessageData.message, message);
            ActionData roomUserMessageActionData(0x55, &roomUserMessageData, 4+(::wcslen(message)+1)*2);
            ActionDispatcher::prepare().action(roomUserMessageActionData).to(RoomDestination(room)).send();
        }
    }

    void RoomService::startMatch(Room& room) {
        room.startMatch();

        std::srand(std::time(nullptr)); // use current time as seed for random generator
        int startRandomSeed = std::rand();
        ActionData matchStartedActionData(0x33, (u8 *) &startRandomSeed, 4);
        ActionDispatcher::prepare().action(matchStartedActionData).to(RoomDestination(room)).send();

        //TODO(CGR): check if game started successfully
        notifyServerOfRoomStateUpdate(room.server(), room);
    }

    void RoomService::userFinishedPlaying(Room& room, const User& user) {
        room.setUserFinishedPlaying((User&)user);

        //TODO(CGR): change to my own way of doing it in future versions
        //This is the Official way, and it's more "hackeable".
        if(room.playingUserCount() == 0) {
            room.setState(Room::RoomState::IDLE);

            //0x50 forces player to exit room (set room state)
            int state = Room::RoomState::IDLE;
            ActionData setRoomStateActionData(0x50, &state, 4);
            ActionDispatcher::prepare().action(setRoomStateActionData).to(RoomDestination(room)).send();

            this->notifyServerOfRoomStateUpdate(*user.server(), room);
        }

    }

    void RoomService::setUserOutOfGameScreen(Room& room, User& user) {
        room.setUserOutOfGameScreen(user);

        if(room.inGameScreenUserCount() == 0) {
            this->resetRoom(room);
        }

        ActionData showRoomScreenActionData(0x61);
        user.client().sendAction(showRoomScreenActionData);
    }

    //TODO(CGR): All following methods should be moved to ServerService?
    void RoomService::notifyServerOfRoomCreation(const GameServer& server, const Room& room) {
        const Room::GameInfo& game = room.gameInfo();
        RoomInfoActionData actionData;
        actionData.roomId = room.id();
        actionData.difficulty = game.difficulty;
        ::wcsncpy(actionData.roomName, room.title(), ROOM_TITLE_MAX_LEN);
        actionData.roomName[ROOM_TITLE_MAX_LEN] = L'\0';
        actionData.playersIn = room.usersInCount();
        actionData.maxPlayers = room.maxPlayers();
        actionData.u = 3;
        actionData.levelLimit = 10;
        actionData.gameType = game.gameType;
        actionData.roomType = game.roomType;
        actionData.matchType = game.matchType;
        actionData.roomState = room.inGame();
        ::wcsncpy(actionData.roomMaster, room.roomMaster()->player()->name(), PLAYER_NAME_MAX_LEN);
        actionData.roomMaster[PLAYER_NAME_MAX_LEN] = L'\0';
        actionData.straightWins = room.straightWins();
        actionData.caneys = game.caneys;
        ActionData action(0x2A, &actionData, sizeof(actionData));
        ActionDispatcher::prepare().action(action).to(ServerDestination(server, 1)).send();
    }

	void RoomService::notifyServerOfRoomMasterChange(const GameServer& server, const Room& room) {
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

    void RoomService::notifyServerOfRoomPlayerCountUpdate(const GameServer& server, const Room& room) {
        struct RoomUpdateInfo {
            u32 roomId;
            int playerCount;
        } updateInfo = { room.id(), room.usersInCount() };
        ActionData action(0x2F, &updateInfo, sizeof(updateInfo));
        ActionDispatcher::prepare().action(action).to(ServerDestination(server, 1)).send();
    }

    void RoomService::notifyServerOfRoomStateUpdate(const GameServer& server, const Room& room) {
        struct RoomUpdateInfo {
            u32 roomId;
            int state;
        } updateInfo = { room.id(), room.closed() };
        int playerInCount = room.usersInCount();
        ActionData action(0x30, &updateInfo, sizeof(updateInfo));
        ActionDispatcher::prepare().action(action).to(ServerDestination(server, 1)).send();
    }

    void RoomService::resetRoom(Room& room) {
        room.resetUserFromSlots();
        updateRoom(room);
    }

    void RoomService::updateRoom(Room& room) {
        bool needsUserUpdateNotification = room.getSlotUserListIndex(ROOM_MASTER_SLOT) == -1;

        room.setUserToSlot(room.roomMasterListIndex(), ROOM_MASTER_SLOT);
        if(room.gameInfo().matchType == MatchType::MATCH_CHALLENGE)
            needsUserUpdateNotification |= updateChallenge(room);

        if(needsUserUpdateNotification)
            updateSlotInfo(room);
    }

    bool RoomService::updateChallenge(Room& room) {
        const std::vector<int>& userQueue = room.userQueue();
        if(userQueue.size() == 0)
            return false;

        bool needsUpdate = room.getSlotUserListIndex(0) == -1;
        room.setUserToSlot(userQueue[0], 0);
        if(userQueue.size() > 1) {
            room.setUserToSlot(userQueue[1], 6);
            needsUpdate = true;
        }

        return needsUpdate;
    }

}