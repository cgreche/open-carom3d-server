//
// Created by CGR on 17/05/2020.
//

#include <ctime>
#include <core/server/client_session.h>
#include <business/api/messaging.h>
#include <business/game_server/game_server.h>
#include <business/util/abstract_action.h>
#include <business/util/action_dispatcher.h>
#include <business/util/destination.h>
#include <business/service/account_service.h>
#include "business/service/channel_service.h"
#include "business/service/room_service.h"
#include "user_service.h"


namespace business {

    static UserService g_userService;

    UserService &UserService::getInstance() {
        return g_userService;
    }

    void UserService::linkClientToUser(core::ClientSession &client, User &user) {
        m_clientsUsers[client.sessionId()] = &user;
    }

    User *UserService::createUserSession(core::ClientSession &clientSession) {
        User *user = new User(clientSession);
        linkClientToUser(clientSession, *user);
        return user;
    }

    User *UserService::getUser(core::ClientSession &clientSession) {
        return m_clientsUsers[clientSession.sessionId()];
    }

    void UserService::loginUser(User& user, const wchar_t *accountId, const wchar_t *accountPassword) {
        Account* account = AccountService::getInstance().logAccount(accountId, accountPassword);
        if(account != nullptr) {
            user.setAccount(account);
        }
    }

    void UserService::logoutUser(User &user) {
        m_clientsUsers.erase(user.client().sessionId());
    }

    //TODO: force joining somewhere (a similar channel)
    Channel *UserService::joinChannel(User &user, const wchar_t *channelName, bool createIfNotExists) {
		this->removeUserFromCurrentSpot(user);
        Channel *channel = ChannelService::getInstance().moveUserToChannel(&user, channelName, createIfNotExists);
        return channel;
    }

    Room *UserService::createRoom(User &user, const CreateRoomActionData &createRoomActionData) {
		this->removeUserFromCurrentSpot(user);

        Room::GameInfo gameInfo;
        gameInfo.gameType = createRoomActionData.gameType;
        gameInfo.matchType = createRoomActionData.matchType;
        gameInfo.roomType = createRoomActionData.roomType;
        gameInfo.difficulty = createRoomActionData.difficulty;
        gameInfo.caneys = createRoomActionData.caneys;
        Room* createdRoom = RoomService::getInstance().createRoom(&user, createRoomActionData.roomTitle, createRoomActionData.roomPassword, 10, gameInfo);
        if(nullptr != createdRoom) {
            notifyServerOfRoomCreation(*user.server(), *createdRoom);
        }
        return createdRoom;
    }

    Room *UserService::joinRoom(User &user, const wchar_t *roomTitle, const wchar_t *roomPassword) {
        Room *room = RoomService::getInstance().getRoom(*user.server(), roomTitle);
        if(!room) {
            //TODO(CGR): send error
            return nullptr;
        }
		//TODO(CGR): check if room has password

		this->removeUserFromCurrentSpot(user);

        RoomService::getInstance().insertUserIntoRoom(*room, user);
        //TODO(CGR): process fail result
        notifyChannelOfRoomPlayerCountUpdate(*user.server(), *room);
        return room;
    }

    void UserService::exitRoom(User &user) {
        Room* room = user.roomIn();
        if(!room)
            return;

        User* roomMaster = room->roomMaster();

        RoomService::getInstance().removeUserFromRoom(*room, user);

        if(room->usersInCount() != 0) {
			//TODO(CGR): move to RoomService
            notifyChannelOfRoomPlayerCountUpdate(*user.server(), *room);
            if(roomMaster == &user)
                notifyChannelOfRoomMasterUpdate(*user.server(), *room);
        }

        Channel* lastChannel = ChannelService::getInstance().getChannel(user.lastChannelName());
        if(nullptr != lastChannel) {
            ChannelService::getInstance().insertUserIntoChannel(*lastChannel, user);
            this->updateUserWithAllServerRooms(user);
        }
    }

    void UserService::joinRoomSlot(User &user, int slot) {
        Room* room = user.roomIn();
        //TODO(CGR): player must not be master
        RoomService::getInstance().setUserToSlot(*room, slot, &user);
    }

    void UserService::exitRoomSlot(User &user) {
        Room* room = user.roomIn();
        //TODO(CGR): player must not be master
        //TODO(CGR): assert player in slot
        int userSlot = room->getUserSlot(user);
        RoomService::getInstance().setUserToSlot(*room, userSlot, nullptr);
    }

    void UserService::setRoomSlotState(User &user, int slot, int state) {
        Room* room = user.roomIn();
        //TODO(CGR): assert player is master
        RoomService::getInstance().setSlotState(*room, slot, (Room::SlotState)state);
    }

    void UserService::changeRoomState(User& user, bool open) {
        Room* room = user.roomIn();
        if(!room || room->roomMaster() != &user)
            return;
        RoomService::getInstance().changeRoomState(*room, open);
    }

    void UserService::kickUserFromRoom(User& user, int userListIndex) {
        Room* room = user.roomIn();
        if(!room || room->roomMaster() != &user)
            return;
        User* userToKick = room->userInListIndex(userListIndex);
        if(!userToKick)
            return;
        RoomService::getInstance().removeUserFromRoom(*room, userListIndex);

        //TODO(CGR): modularize
        if(room->usersInCount() != 0) {
            //TODO(CGR): move to RoomService
            notifyChannelOfRoomPlayerCountUpdate(*user.server(), *room);
            if(room->roomMaster() == userToKick)
                notifyChannelOfRoomMasterUpdate(*user.server(), *room);
        }

        Channel* lastChannel = ChannelService::getInstance().getChannel(userToKick->lastChannelName());
        if(nullptr != lastChannel) {
            ChannelService::getInstance().insertUserIntoChannel(*lastChannel, *userToKick);
            this->updateUserWithAllServerRooms(*userToKick);
        }

    }

    void UserService::sendMessageToRoom(User &user, const wchar_t *message) {
        Room* room = user.roomIn();
        if(!room)
            return;

		//if (::wcsstr(message, L"-d") != 0) {
		//	wchar_t rm[100];
		//	::wcscpy(rm, &message[8]);
		//	ActionData roomMaster(0x32, rm, sizeof(rm)-8);
		//	ActionDispatcher::prepare().action(roomMaster).to(RoomDestination(*room)).send();
		//}

        RoomService::getInstance().sendMessageToRoom(*room, user, message);
    }

    void UserService::startMatch(User& user) {
        Room* room = user.roomIn();
        if(!room || room->roomMaster() != &user)
            return;
        RoomService::getInstance().startGame(room);

        //TODO(CGR): check if game started
        notifyChannelOfRoomStateUpdate(*user.server(), *room);
    }

    void UserService::sendMatchEventInfo(User& user, const u8* data, u32 dataSize) {
        Room* room = user.roomIn();
        if(!room)
            return;

        ActionData matchEventInfoAction(0x47, data, dataSize);
        for(auto userListIndex : room->userListIndexes()) {
            User* userIn = room->userInListIndex(userListIndex);
            if(userIn != &user) {
                ActionDispatcher::prepare()
                                    .action(matchEventInfoAction)
                                    .to(UserDestination(*userIn))
                                    .send();
            }
        }
    }

	void UserService::requestMatchMakerScreen(User& user) {
		Room* room = user.roomIn();
		if(nullptr == room)
			return;

		room->setUserOutGame(user);

		user.client().sendAction(ActionData(0x61));
		if (room->inGameUserCount() == 0) {
			room->setState(Room::RoomState::IDLE); //TODO(CGR): this should not be here...
			//room->resetSlotPlayerIndexes();
			RoomService::getInstance().updateSlotInfo(*room);

			this->notifyChannelOfRoomStateUpdate(*user.server(), *room);
		}

	}

    //TODO(CGR): Move to ServerService?
    void UserService::notifyServerOfRoomCreation(const GameServer& server, const Room& room) {
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

    void UserService::updateUserWithAllServerRooms(const User& user) {
        GameServer* gameServer = user.server();
        for(auto room : gameServer->rooms()) {
            const Room::GameInfo& game = room->gameInfo();
            RoomInfoActionData actionData;
            actionData.roomId = room->id();
            actionData.difficulty = game.difficulty;
            ::wcsncpy(actionData.roomName, room->title(), ROOM_TITLE_MAX_LEN);
            actionData.roomName[ROOM_TITLE_MAX_LEN] = L'\0';
            actionData.playersIn = room->usersInCount();
            actionData.maxPlayers = room->maxPlayers();
            actionData.u = 10;
            actionData.levelLimit = 12;
            actionData.gameType = game.gameType;
            actionData.roomType = game.roomType;
            actionData.matchType = game.matchType;
            actionData.roomState = room->inGame() ? 2 : 1;
            User* roomMaster = room->roomMaster();
            ::wcsncpy(actionData.roomMaster, roomMaster ? roomMaster->player()->name() : L"", PLAYER_NAME_MAX_LEN);
            actionData.roomMaster[PLAYER_NAME_MAX_LEN] = L'\0';
            actionData.straightWins = room->straightWins();
            actionData.caneys = game.caneys;

            ActionData roomInfoAction(0x2B, &actionData, sizeof(actionData));
            ActionDispatcher::prepare().action(roomInfoAction).to(UserDestination(user)).send();
        }

        ActionData roomInfoAction(0x62);
        ActionDispatcher::prepare().action(roomInfoAction).to(UserDestination(user)).send();
    }

    void UserService::notifyChannelOfRoomMasterUpdate(const GameServer& server, const Room& room) {
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

    void UserService::notifyChannelOfRoomPlayerCountUpdate(const GameServer& server, const Room& room) {
        struct RoomUpdateInfo {
            u32 roomId;
            int playerCount;
        } updateInfo = { room.id(), room.usersInCount() };
        ActionData action(0x2F, &updateInfo, sizeof(updateInfo));
        ActionDispatcher::prepare().action(action).to(ServerDestination(server, 1)).send();
    }

    void UserService::notifyChannelOfRoomStateUpdate(const GameServer& server, const Room& room) {
        struct RoomUpdateInfo {
            u32 roomId;
            int state;
        } updateInfo = { room.id(), room.closed() };
        int playerInCount = room.usersInCount();
        ActionData action(0x30, &updateInfo, sizeof(updateInfo));
        ActionDispatcher::prepare().action(action).to(ServerDestination(server, 1)).send();
    }

    void UserService::removeUserFromCurrentSpot(User& user) {
        if(nullptr == user.spot())
            return;

        if(user.spot()->isOfType(0))
            ChannelService::getInstance().removeUserFromChannel(*(Channel*)user.spot(), user);
        else if(user.spot()->isOfType(1))
            RoomService::getInstance().removeUserFromRoom(*(Room*)user.spot(), user);
    }

}