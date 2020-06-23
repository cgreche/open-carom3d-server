//
// Created by CGR on 19/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_ROOM_H__
#define __OPEN_CAROM3D_SERVER_ROOM_H__

#include <vector>
#include <business/api/constants.h>
#include <core/lib/nettools/ntConnection.h>
#include "player.h"
#include "user.h"
#include "user_spot.h"
#include "match.h"

namespace business {

    class Match;

    class Room : public UserSpot {
    public:
        enum SlotState {
            DISABLED = 0,
            OPEN,
            CLOSED
        };

        enum RoomState {
            IDLE = 1,
            IN_GAME = 2
        };

        struct SlotInfos {
            SlotState state[ROOM_SLOT_COUNT];
            int playerListIndex[ROOM_SLOT_COUNT];
        };

		struct RoomUser {
            User *user;
            int slot;
            bool inGameScreen;
			bool playing;
        };

        struct GameInfo {
            int roomType;
            int gameType;
            int difficulty;
            int matchType;
            int caneys;
        };

    private:
		GameServer& m_server;

        u32 m_id;
        std::wstring m_title;
        std::wstring m_password;
        std::vector<RoomUser> m_roomUsers;
        SlotInfos m_slots;

        int m_usersIn;
        int m_maxUsers;
        GameInfo m_gameInfo;
        int m_straightWins;

        RoomState m_state;
        bool m_closed;

        Match *m_matchInfo;
		int m_playingUserCount;
        int m_inGameScreenUserCount;

        User *m_creator;
        int m_roomMasterListIndex;
        std::vector<int> m_userQueue;
		std::vector<User*> m_users;

        int getFreeListIndex() const;
        int geNextRoomMaster() const;

        //TODO(CGR): create method for checking teams integrity
        void constructMatchInfo();

    public:
        Room(GameServer& server, u32 id, const wchar_t* title, User* creator, u32 maxPlayers, const Room::GameInfo& gameInfo);

        bool isOfType(int type) override;
        const wchar_t* description() const override;
        const wchar_t *name() const override;

        int insertUser(User& user) override;
        void removeUser(User& user) override;
        void removeUser(int userListIndex);

        bool isUserIn(const wchar_t* userName) override;

		unsigned int userCount() override;

        //Room specific
        int setUserToSlot(int slot, int userListIndex);
        int setUserToSlot(User& user, int slot);
        int freeSlot(int slot) { return setUserToSlot(-1, slot); }
        int setSlotState(int slot, Room::SlotState state);
        int setSlotStates(Room::SlotState state[ROOM_SLOT_COUNT]);

        int updateRoomMaster();

        int getUserListIndex(User &user) const;
        int getUserSlot(User &user) const;
        int getSlotUserListIndex(int slot) const { return m_slots.playerListIndex[slot]; }
        int getSlotState(int slot) const { return m_slots.state[slot]; }

        void setTitle(const wchar_t *title) { m_title = title; }
        void setPassword(const wchar_t *password) { m_password = password; }
		void setState(RoomState state) { m_state = state; }
        void setClosed(bool closed) { m_closed = closed; }

		GameServer& server() { return m_server; }
        u32 id() const { return m_id; }
        const wchar_t *password() const { return m_password.c_str(); }
        User *roomMaster() const { return m_roomMasterListIndex != -1 ? m_roomUsers[m_roomMasterListIndex].user : nullptr; }
        int roomMasterListIndex() const { return m_roomMasterListIndex;  }
        int usersInCount() const { return m_usersIn; }
        int maxPlayers() const { return m_maxUsers; }
        const GameInfo& gameInfo() const { return m_gameInfo; }
        const std::vector<int>& userQueue() const { return m_userQueue; }
        const std::vector<User*> users() const { return m_users; }
        User* userInListIndex(int listIndex) const;
        int straightWins() const { return m_straightWins; }
        RoomState state() const { return m_state; }

        const SlotInfos& slotInfos() const { return m_slots; }
        bool closed() const { return m_closed; }

        bool inGame() const { return m_state == RoomState::IN_GAME; }

        u32 startMatch();
        u32 endMatch();
        void setUserFinishedPlaying(User& user);
		void setUserOutOfGameScreen(User& user);

        int playingUserCount() const { return m_playingUserCount; }
        int inGameScreenUserCount() const { return m_inGameScreenUserCount; }

        int resetUserFromSlot(int slot);
        void resetUserFromSlots();
    };

}

#endif //__OPEN_CAROM3D_SERVER_ROOM_H__
