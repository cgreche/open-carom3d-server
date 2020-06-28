//
// Created by CGR on 19/05/2020.
//

#include <ctime>
#include <business/api/messaging.h>
#include <business/entity/player.h>
#include <business/entity/room.h>
#include <business/game_server/game_server.h>
#include <business/game_server/send_actions.h>
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

    RoomService &RoomService::getInstance() {
        return g_roomService;
    }

    Room* RoomService::createRoom(User *user, const wchar_t *title, const wchar_t *password, int maxPlayers, const Room::GameInfo& gameInfo) {
        Room* newRoom;
        user->server().createRoom(title, user, maxPlayers, gameInfo, &newRoom);
        if(nullptr == newRoom)
            return nullptr;

        newRoom->setPassword(password);

        const int* slotStatesLayout = nullptr;

        //TODO(CGR): create a MATCH_TYPE enum
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
            slotStatesLayout = NORMAL_GAME_SLOT_LAYOUT;
            break;
        default:
            break;
        }
        newRoom->setSlotStates((Room::SlotState*)slotStatesLayout);

        int listIndex = newRoom->insertUser(*user);
        user->setSpot(newRoom);
        newRoom->setUserToSlot(listIndex, ROOM_MASTER_SLOT);


        user->sendAction(RoomCreationActionTemplate(*newRoom).data());
        //TODO(CGR): room creation fail

        this->resetRoom(*newRoom);

        RoomPlayerItemData item = {0};
        ActionData playerItemInfosActionData(0x71, &item, sizeof(item));
        ActionDispatcher::prepare()
                .action(RoomPlayerJoinActionTemplate(*user->player(), listIndex).data())
                .action(playerItemInfosActionData)
                .send(UserDestination(*user));

        ActionData playerListEnd(0x63);
        ActionDispatcher::prepare().action(playerListEnd).send(UserDestination(*user));

        notifyServerOfRoomCreation(newRoom->server(), *newRoom);
        return newRoom;
    }

    void RoomService::insertUserIntoRoom(Room &room, User& user) {
        int listIndex = room.insertUser(user);
        user.setSpot(&room);

        updateRoom(room);
        user.sendAction(RoomJoinActionTemplate(room).data());

        //get all players in room
        for(auto userListIndex : room.userQueue()) {
            User *userIn = room.userInListIndex(userListIndex);
            if(userIn == &user)
                continue;

            RoomPlayerItemData item = {0};
            ActionData playerItemInfosActionData(0x71, &item, sizeof(item));

            ActionDispatcher::prepare().action(RoomPlayerJoinActionTemplate(*userIn->player(), userListIndex).data()).send(UserDestination(user));
            ActionDispatcher::prepare().action(playerItemInfosActionData).send(UserDestination(user));
        }

        //notify all players that user joined
        RoomPlayerItemData item = {0};
        ActionData playerItemInfosActionData(0x71, &item, sizeof(item));
        ActionDispatcher::prepare()
                            .action(RoomPlayerJoinActionTemplate(*user.player(), listIndex).data())
                            .action(playerItemInfosActionData)
                            .send(RoomDestination(room));

        ActionData playerListEnd(0x63, nullptr, 0);
        ActionDispatcher::prepare().action(playerListEnd).send(UserDestination(user));

        //TODO(CGR): process fail result
        notifyServerOfRoomPlayerCountUpdate(user.server(), room);
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
		ActionDispatcher::prepare().action(exitRoomAction).send(UserDestination(*user));

		if (room.usersInCount() != 0) {
			if (currentRoomMaster != newRoomMaster) {
                if(!room.inGame())
                    updateSlotInfo(room);

				ActionData changeRoomMasterAction(0x32, newRoomMaster->player()->name(), (PLAYER_NAME_MAX_LEN + 1)*2);
				ActionDispatcher::prepare().action(changeRoomMasterAction).send(RoomDestination(room));
				notifyServerOfRoomMasterChange(room.server(), room);
			}

			int listIndex = userListIndex;
			ActionData userExitedRoomAction(0x28, &listIndex, 4);
			ActionDispatcher::prepare().action(userExitedRoomAction).send(RoomDestination(room));

            notifyServerOfRoomPlayerCountUpdate(room.server(), room);
        }
		else {
			int roomId = room.id();

			GameServer& server = room.server();
			server.destroyRoom(room);

			ActionData destroyRoomAction(0x2D, &roomId, 4);
			ActionDispatcher::prepare().action(destroyRoomAction).send(ServerDestination(server, 1));
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
            userIn->sendAction(playerJoinSlotAction);
        }
    }

    void RoomService::setSlotState(Room& room, int slot, Room::SlotState state) {
        room.setSlotState(slot, state);

        //TODO(CGR): modularize
        //TODO(CGR): set to non-literal constant
        int listIndex = room.getSlotUserListIndex(slot);
        int slotState = room.getSlotState(slot);
        SlotModificationResultData modificationResultData = {listIndex, slot, (int)slotState};
        ActionData slotStateActionData(0x4D, (u8 *)&modificationResultData, sizeof(modificationResultData));
        ActionDispatcher::prepare().action(slotStateActionData).send(RoomDestination(room));
    }

	void RoomService::updateSlotInfo(Room& room) {
		RoomSlotInfoData slotInfoData;
		const Room::SlotInfos& slotInfos = room.slotInfos();
		for (int i = 0; i < 30; ++i) {
			slotInfoData.slotState[i] = slotInfos.state[i];
			slotInfoData.playerListIndex[i] = slotInfos.playerListIndex[i];
		}
		ActionData slotInfoActionData(0x51, &slotInfoData, sizeof(slotInfoData));
		ActionDispatcher::prepare().action(slotInfoActionData).send(RoomDestination(room));
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
                wchar_t message[151];
            };
#pragma pack(pop)
            RoomUserMessage roomUserMessageData;
            roomUserMessageData.userListIndex = userListIndex;
            ::wcscpy(roomUserMessageData.message, message);
            ActionData roomUserMessageActionData(0x55, &roomUserMessageData, 4+(::wcslen(message)+1)*2);
            ActionDispatcher::prepare().action(roomUserMessageActionData).send(RoomDestination(room));
        }
    }

    void RoomService::startMatch(Room& room) {
        room.startMatch();

        std::srand(std::time(nullptr)); // use current time as seed for random generator
        int startRandomSeed = std::rand();
        ActionData matchStartedActionData(0x33, (u8 *) &startRandomSeed, 4);
        ActionDispatcher::prepare().action(matchStartedActionData).send(RoomDestination(room));

        //TODO(CGR): check if game started successfully
        notifyServerOfRoomStateUpdate(room.server(), room);
    }

    void RoomService::userFinishedPlaying(Room& room, const User& user) {
        room.setUserFinishedPlaying((User&)user);

        //TODO(CGR): change to my own way of doing it in future versions
        //This is the Official way, and it's more "hackeable".
        if(room.playingUserCount() == 0) {
            room.setState(Room::RoomState::IDLE);

            //0x50 forces player to exit room (set room state to client)
            int state = Room::RoomState::IDLE;
            ActionData setRoomStateActionData(0x50, &state, 4);
            ActionDispatcher::prepare().action(setRoomStateActionData).send(RoomDestination(room));

            this->notifyServerOfRoomStateUpdate(user.server(), room);
        }

    }

    void RoomService::setUserOutOfGameScreen(Room& room, User& user) {
        room.setUserOutOfGameScreen(user);

        ActionData showRoomScreenActionData(0x61);
        user.sendAction(showRoomScreenActionData);

        if(room.inGameScreenUserCount() == 0) {
            this->resetRoom(room);
        }
    }

    //TODO(CGR): All following methods should be moved to ServerService?
    void RoomService::notifyServerOfRoomCreation(const GameServer& server, const Room& room) {

        ActionDispatcher::prepare()
            .action(RoomCreationNotifyActionTemplate(room).data())
            .send(ServerDestination(server, 1));

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
		ActionDispatcher::prepare().action(action).send(ServerDestination(server, 1));
	}

    void RoomService::notifyServerOfRoomPlayerCountUpdate(const GameServer& server, const Room& room) {
        struct RoomUpdateInfo {
            u32 roomId;
            int playerCount;
        } updateInfo = { room.id(), room.usersInCount() };
        ActionData action(0x2F, &updateInfo, sizeof(updateInfo));
        ActionDispatcher::prepare().action(action).send(ServerDestination(server, 1));
    }

    void RoomService::notifyServerOfRoomStateUpdate(const GameServer& server, const Room& room) {
        struct RoomUpdateInfo {
            u32 roomId;
            int state;
        } updateInfo = { room.id(), room.state() };
        int playerInCount = room.usersInCount();
        ActionData action(0x30, &updateInfo, sizeof(updateInfo));
        ActionDispatcher::prepare().action(action).send(ServerDestination(server, 1));
    }

    void RoomService::resetRoom(Room& room) {
        room.resetUserFromSlots();
        updateRoom(room);
    }

    void RoomService::updateRoom(Room& room) {
        if(room.state() == Room::RoomState::IN_GAME)
            return;

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