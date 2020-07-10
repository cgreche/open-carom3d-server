//
// Created by CGR on 19/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_ROOM_SERVICE_H__
#define __OPEN_CAROM3D_SERVER_ROOM_SERVICE_H__

namespace business {

    class User;
    class Room;

    class RoomService {

        void resetRoom(Room& room);
        void updateRoom(Room& room);
        bool updateChallenge(Room& room);

    public:
        static RoomService& getInstance();

        Room* createRoom(User* user, const wchar_t* title, const wchar_t* password, int maxPlayers, const Room::GameInfo& gameInfo);
        Room* getRoom(GameServer& server, const wchar_t* roomTitle);
        void insertUserIntoRoom(Room& room, User& user);
        void removeUserFromRoom(Room& room, User& user);
        void removeUserFromRoom(Room& room, int userListIndex);
        void setUserToSlot(Room& room, int slot, User* user);
        void setSlotState(Room& room, int slot, Room::SlotState state);
		void updateSlotInfo(Room& room);
        void changeRoomState(Room& room, bool open);
        void sendMessageToRoom(Room& room, User& sender, const wchar_t* message);
        void startMatch(Room& room);
        void userFinishedPlaying(Room& room, const User& user);
        void setUserOutOfGameScreen(Room& room, User& user);

        void notifyServerOfRoomCreation(const GameServer& server, const Room& room);
		void notifyServerOfRoomMasterChange(const GameServer& server, const Room& room);
        void notifyServerOfRoomPlayerCountUpdate(const GameServer& server, const Room& room);
        void notifyServerOfRoomStateUpdate(const GameServer& server, const Room& room);

    };

}

#endif //__OPEN_CAROM3D_SERVER_ROOM_SERVICE_H__
