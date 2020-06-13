//
// Created by CGR on 16/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_USER_SERVICE_H__
#define __OPEN_CAROM3D_SERVER_USER_SERVICE_H__

#include <list>
#include <map>
#include <vector>
#include <business/api/messaging.h>

namespace core { class ClientSession; }

namespace business {

    class User;
    class Account;
    class Player;
    class Channel;
    class GameServer;
    class Room;

    class UserService {

        std::map<int, User *> m_clientsUsers;
        std::map<Account*, User*> m_usersAccounts;

    public:
        static UserService &getInstance();

        void loginUser(User& user, const wchar_t *acocuntId, const wchar_t *accountPassword, const wchar_t *preferredLanguage);
        void logoutUser(User &user);
        User *getUser(core::ClientSession &clientSession);

        Channel *joinChannel(User &user, const wchar_t *channelName);
        Room *createRoom(User &user, const CreateRoomActionData &createRoomActionData);
		void sendNotifyMessage(User& user, const wchar_t* message);
        Room *joinRoom(User &user, const wchar_t* roomTitle, const wchar_t* roomPassword);
        void exitRoom(User &user);
        void joinRoomSlot(User& user, int slot);
        void exitRoomSlot(User& user);
        void setRoomSlotState(User& user, int slot, int state);
        void changeRoomState(User& user, bool open);
        void kickUserFromRoom(User& user, int userListIndex);
        void sendMessageToRoom(User& user, const wchar_t* message);
        void startMatch(User& user);
        void matchFinished(User& user);
        void sendMatchEventInfo(User& user, const u8* data, u32 dataSize);
        void sendMatchEventInfo2(User& user, const u8* data, u32 dataSize);
        void requestMatchMakerScreen(User& user);

        //
        void updateUserWithAllServerRooms(const User& user);

		//Utils
		void removeUserFromCurrentSpot(User& user);

    };

}

#endif //__OPEN_CAROM3D_SERVER_USER_SERVICE_H__
