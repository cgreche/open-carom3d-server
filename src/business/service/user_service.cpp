//
// Created by CGR on 17/05/2020.
//

#include <ctime>
#include <thread>
#include <core/server/client_session.h>
#include <business/api/messaging.h>
#include <business/game_server/game_server.h>
#include <business/game_server/send_actions.h>
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

    User* UserService::getUser(core::ClientSession& clientSession) {
        return m_clientsUsers[clientSession.sessionId()];
    }

    User* UserService::findUser(const wchar_t* userName) {
        Account* account = AccountService::getInstance().findAccount(userName);
        if(nullptr == account)
            return nullptr;

        return m_usersAccounts[account];
    }

	void sendClientVersion(User& user) {
		char version[20] = "5.31";
		ActionData action(0x16, (unsigned char*)version, sizeof(version));
		user.sendAction(action);
	}

	void sendLoginResult(User& user) {
		LoginResult loginResult = {
				0,
				L"",
				(unsigned long)user.account()->id(),
				100
		};
		::wcsncpy(loginResult.playerName, user.player()->name(), 20);
		loginResult.playerName[20] = L'\0';

		ActionData loginResultAction(0x18, (unsigned char*)&loginResult, sizeof(loginResult));
		user.sendAction(loginResultAction);
	}

	//TODO(CGR): move to a more appropriated service
	void sendBlackBoardMessage(User& user, const wchar_t* message) {
		ActionData blackBoardMessageAction(0x56, (unsigned char*)message, (::wcslen(message) + 1) * 2);
		user.sendAction(blackBoardMessageAction);
	}

	//TODO(CGR): move to a more appropriated service
	void sendElectronicPanelMessage(User& user, const wchar_t* message) {
		ActionData action(0x54, (unsigned char*)message, (::wcslen(message) + 1) * 2);
		user.sendAction(action);
	}

    void UserService::loginUser(User& user, const wchar_t *accountId, const wchar_t *accountPassword, const wchar_t *preferredLanguage) {
		Account* account = AccountService::getInstance().logAccount(accountId, accountPassword);
		if (account == nullptr)
			return;

        User* loggedUser = m_usersAccounts[account];
        if(nullptr != loggedUser) {
            //Send message to user;
            ActionData disconnectAction(0x35);
            ActionDispatcher::prepare().action(disconnectAction).send(UserDestination(*loggedUser));

            loggedUser->server().disconnectClient(loggedUser);
        }

        m_usersAccounts[account] = &user;

		printf("Player logged in: %ws", account->name());
		user.setAccount(account);
		user.setPlayer(account->player());

		sendClientVersion(user);

		int waitValue = 0;
		ActionData notifyRemainAction(0x60, (unsigned char*)&waitValue, 4);
		user.sendAction(notifyRemainAction);

		//TODO(CGR): sleeping is bad. But without it, messages aren't processed correctly on client
		// Find a better way to do it
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		sendLoginResult(user);

		//TODO(CGR): process login result
		// If login result == 0
		sendBlackBoardMessage(user, L"Welcome to Open Carom3D (v 0.2.0 Alpha)");
		sendElectronicPanelMessage(user, L"Hello World");

		PlayerStats playerStats = { 0 };
		playerStats.country[0] = L'B';
		playerStats.country[1] = L'R';
		ActionData playerStatsAction(0x68, (unsigned char*)&playerStats, sizeof(playerStats));
		user.sendAction(playerStatsAction);

		sendNotifyMessage(user, L"Welcome to Open Carom3D Server\n");

		std::wstring generatedChannelName = L"Carom ";
		generatedChannelName += preferredLanguage;
		generatedChannelName += L"-1";

		this->joinChannel(user, generatedChannelName.c_str());
    }

    void UserService::logoutUser(User &user) {
        removeUserFromCurrentSpot(user);
        if(nullptr != user.account())
            m_usersAccounts[user.account()] = nullptr;
        m_clientsUsers.erase(user.sessionId());
    }

	void UserService::sendNotifyMessage(User& user, const wchar_t* message) {
		ActionData greenMessage(0x1C, (unsigned char*)message, (::wcslen(message) + 1) * 2);
		user.sendAction(greenMessage);
	}

    //TODO: force joining somewhere (a similar channel)
    Channel *UserService::joinChannel(User &user, const wchar_t *channelName) {
		this->removeUserFromCurrentSpot(user);
        Channel *channel = ChannelService::getInstance().moveUserToChannel(&user, channelName, true);
        this->updateUserWithAllServerRooms(user);
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
        return createdRoom;
    }

    Room *UserService::joinRoom(User &user, const wchar_t *roomTitle, const wchar_t *roomPassword) {
        Room *room = RoomService::getInstance().getRoom(user.server(), roomTitle);
        if(!room) {
            //TODO(CGR): send error
            return nullptr;
        }
		//TODO(CGR): check if room has password

		this->removeUserFromCurrentSpot(user);
        RoomService::getInstance().insertUserIntoRoom(*room, user);
        return room;
    }

    void UserService::exitRoom(User &user) {
        Room* room = user.roomIn();
        if(!room)
            return;
        this->joinChannel(user, user.lastChannelName());
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

        this->joinChannel(*userToKick, userToKick->lastChannelName());
    }

    void UserService::sendMessageToRoom(User &user, const wchar_t *message) {
        Room* room = user.roomIn();
        if(!room)
            return;

		if (::wcsstr(message, L"-s") != 0) {
            this->startMatch(user);
		}

        RoomService::getInstance().sendMessageToRoom(*room, user, message);
    }

    void UserService::startMatch(User& user) {
        Room* room = user.roomIn();
        if(!room || room->roomMaster() != &user)
            return;
        RoomService::getInstance().startMatch(*room);
    }

    void UserService::matchFinished(User& user) {
        Room* room = user.roomIn();
        if(nullptr == room)
            return;

        RoomService::getInstance().userFinishedPlaying(*room, user);
    }

    void UserService::requestPlayerProfile(User& user, const wchar_t* playerName) {
        User* targetUser = findUser(playerName);
        if(nullptr == targetUser) {
            //TODO(CGR): send couldn't find player
            return;
        }

        user.sendAction(PlayerProfileActionTemplate(*targetUser->player()).data());
    }


    void UserService::sendPrivateMessageToUser(User& user, const wchar_t* userName, const wchar_t* message) {
        User* targetUser = findUser(userName);
        u32 result;
        //TODO(CGR): change literal to ALIAS/Enums
        if(nullptr == targetUser)
            result = 1;
        else if(targetUser->inCover(COVER_MESSAGE))
            result = 2;
        else
            result = 0;

        if(result == 0)
            targetUser->sendAction(UserPrivateMessageActionTemplate(userName, message).data());
        user.sendAction(ActionData(0x3D,(u8*)&result,sizeof(u32)));
    }

    void UserService::requestUserSpot(User& user, const wchar_t* userName) {
        User* targetUser = findUser(userName);
        if(nullptr == targetUser || nullptr == targetUser->spot()) {
            user.sendAction(ActionData(0x3E,userName, (PLAYER_NAME_MAX_LEN + 1) * 2));
            return;
        }

        const wchar_t* channelName = L"";
        const wchar_t* roomName = L"";
        const wchar_t* serverName = L"";

        UserSpot* spot = targetUser->spot();
        if(spot->isOfType(0)) {
            channelName = spot->name();
        }
        else if(spot->isOfType(1)) {
            roomName = spot->name();
        }

        serverName = targetUser->server().qualifiedName().data();

        ActionBuilder builder(0x3F);
        builder.add(targetUser->player()->name(), (PLAYER_NAME_MAX_LEN + 1) * 2);
        builder.add(channelName, 31 * 2);
        builder.add(roomName, (40 + 1) * 2);
        builder.add(serverName, (40 + 1) * 2);
        user.sendAction(builder.build());
    }

    void UserService::requestGuildProfile(User& user, const wchar_t* guildName) {

    }

    void UserService::sendGuildMessage(User& user, const wchar_t* message) {

    }
    
    void UserService::requestGuildUserSpots(User& user, const wchar_t* guildName) {

    }

    void UserService::setCoverStates(User& user, const int states[]) {
        user.setCoverStates((int*)states);
    }

    void UserService::sendMatchEventInfo(User& user, const u8* data, u32 dataSize) {
        Room* room = user.roomIn();
        if(!room)
            return;

        u32 first4 = *(u32*)data;

        ActionData matchEventInfoAction(0x47, data, dataSize);
        for(auto userIn : room->users()) {
            if(userIn != &user) {
                ActionDispatcher::prepare()
                    .action(matchEventInfoAction)
                    .send(UserDestination(*userIn));
            }
        }
    }

    void UserService::sendMatchEventInfo2(User& user, const u8* data, u32 dataSize) {
        Room* room = user.roomIn();
        if(!room)
            return;

        u32 enteringUserListIndex = *(u32*)data;
        User* enteringUser = room->userInListIndex(enteringUserListIndex);
        //TODO(CGR): get last joiner, not sent listIndex as a better way to protect

        if(nullptr != enteringUser) {
            ActionData matchEventInfoAction2(0x49, data, dataSize);
            ActionDispatcher::prepare().action(matchEventInfoAction2).send(UserDestination(*enteringUser));
        }
    }

	void UserService::requestMatchMakerScreen(User& user) {
		Room* room = user.roomIn();
		if(nullptr == room)
			return;

        RoomService::getInstance().setUserOutOfGameScreen(*room, user);
	}

    void UserService::inviteUserToRoom(User& user, const wchar_t* userName) {
        Room* room = user.roomIn();
        if(nullptr == room)
            return;

        u32 result;
        User* targetUser = findUser(userName);
        //TODO(CGR): change literals to aliases
        if(nullptr == targetUser)
            result = 1;
        else if(targetUser->inCover(COVER_IN))
            result = 2;
        else if(targetUser->inRoom())
            result = 3;
        else
            result = 0;

        if(result == 0)
            user.sendAction(UserInviteActionTemplate(user, *user.roomIn()).data());

        //0 - Success
        //1 - Player not in this server
        //2 - Player in coverin
        //3 - Player in another room
        //4 - Unknown
        ActionBuilder builder(0x4C);
        builder.add(result);
        user.sendAction(builder.build());
    }

    void UserService::updateUserWithAllServerRooms(const User& user) {
        GameServer& gameServer = user.server();
        for(auto room : gameServer.rooms()) {
            if(room->hidden())
                continue;
            ActionDispatcher::prepare()
                .action(ExistingRoomsNotificationActionTemplate(*room).data())
                .send(UserDestination(user));
        }

        ActionData roomInfoEndAction(0x62);
        ActionDispatcher::prepare().action(roomInfoEndAction).send(UserDestination(user));
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