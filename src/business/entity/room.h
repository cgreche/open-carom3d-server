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

#define ROOM_MASTER_SLOT 0 //TODO(CGR): remove this, turn Room a little more general

    class Match;

    class Room : public UserSpot {
    public:
        enum SlotState {
            DISABLED = 0,
            OPEN,
            CLOSED
        };

        enum RoomState {
            IDLE = 0,
            IN_GAME = 2
        };

        struct SlotInfos {
            SlotState state[ROOM_SLOT_COUNT];
            int playerListIndex[ROOM_SLOT_COUNT];
        };

        struct GameInfo {
            int roomType;
            int gameType;
            int difficulty;
            int matchType;
            int caneys;
        };

    private:
        struct RoomPlayer {
            User *user;
            int slot;
            bool inGame;
        };

		GameServer& m_server;

        u32 m_id;
        std::wstring m_title;
        std::wstring m_password;
        std::vector<RoomPlayer> m_users;
        SlotInfos m_slots{}; //TODO(CGR):
        int m_playersIn{};
        int m_maxPlayers;
        GameInfo m_gameInfo;
        int m_straightWins;

        RoomState m_state;
        bool m_closed;

        Match *m_matchInfo;
		int m_inGameUserCount;

        User *m_creator;
        User *m_roomMaster;
        std::vector<int> m_playerQueue;

        int getFreeListIndex() const;
        int geNextRoomMaster() const;

        //TODO(CGR): create method for checking teams integrity
        void constructMatchInfo();

    public:
        Room(GameServer& server, u32 id, const wchar_t* title, User* creator, u32 maxPlayers, const Room::GameInfo& gameInfo);

        bool isOfType(int type) override;
        const wchar_t* description() override;
        const wchar_t *name() override;

        int insertUser(User& user) override;
        void removeUser(User& user) override;
        void removeUser(int userListIndex);

        bool isUserIn(const wchar_t* userName) override;

        //Room specific
        int setUserToSlot(int slot, int userListIndex);
        int setUserToSlot(int slot, User &user);
        int freeSlot(int slot) { return setUserToSlot(slot, -1); }
        int setSlotState(int slot, Room::SlotState state);
		void resetSlotPlayerIndexes();

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
        const wchar_t *title() const { return m_title.c_str(); }
        const wchar_t *password() const { return m_password.c_str(); }
        User *roomMaster() const { return m_roomMaster; }
        int usersInCount() const { return m_playersIn; }
        int maxPlayers() const { return m_maxPlayers; }
        const GameInfo& gameInfo() const { return m_gameInfo; }
        const std::vector<int>& userListIndexes() const { return m_playerQueue; }
        User* userInListIndex(int listIndex) const;
        int straightWins() const { return m_straightWins; }
        RoomState state() const { return m_state; }

        const SlotInfos& slotInfos() const { return m_slots; }
        bool closed() const { return m_closed; }

        bool inGame() const { return m_state == RoomState::IN_GAME; }

        u32 startGame();
		void setUserOutGame(User& user);

		int inGameUserCount() const { return m_inGameUserCount; }

        int resetUserFromSlot(int slot);
    };

}

#endif //__OPEN_CAROM3D_SERVER_ROOM_H__
