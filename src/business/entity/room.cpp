//
// Created by CGR on 19/05/2020.
//

#include <algorithm>
#include "room.h"
#include "match.h"

namespace business {

    int Room::getFreeListIndex() const {
        for (int i = 0; i < m_roomUsers.size(); ++i) {
            if (m_roomUsers[i].user == nullptr)
                return i;
        }
        return -1;
    }

    int Room::getUserListIndex(User &user) const {
        for (int i = 0; i < m_roomUsers.size(); ++i) {
            if(m_roomUsers[i].user == &user)
                return i;
        }
        return -1;
    }

    int Room::getUserSlot(User &user) const {
        int listIndex = getUserListIndex(user);
        if(listIndex >= 0)
            return m_roomUsers[listIndex].slot;
        return -1;
    }

    int Room::geNextRoomMaster() const {
        if(m_userQueue.empty())
            return -1;
        return m_userQueue[0];
    }

    Room::Room(GameServer& server, u32 id, const wchar_t *title, User *creator, u32 maxUsers, const Room::GameInfo &gameInfo)
        : m_server(server)
		, m_roomUsers(maxUsers) {
        m_id = id;
        m_title = title;
        m_creator = creator;
        m_roomMasterListIndex = -1;
        m_maxUsers = maxUsers;
        m_gameInfo = gameInfo;
        m_closed = false;
        m_matchInfo = nullptr;
        m_straightWins = 0;
        m_usersIn = 0;
        m_state = RoomState::IDLE;
		m_playingUserCount = 0;
        m_inGameScreenUserCount = 0;

        //init user info
        for(auto &user : m_roomUsers) {
            user.user = nullptr;
            user.slot = -1;
			user.inGameScreen = false;
			user.playing = false;
        }

        //initializing slots
        for(int i = 0; i < 30; ++i) {
            m_slots.state[i] = SlotState::DISABLED;
            m_slots.playerListIndex[i] = -1;
        }

		m_userQueue.reserve(maxUsers);
		m_users.reserve(maxUsers);

	}

    bool Room::isOfType(int type)  {
        //TODO(CGR): define type as constants
        return type == 1;
    }

    const wchar_t* Room::description() {
        return L"Room";
    }

    const wchar_t *Room::name() {
        return m_title.c_str();
    }

    int Room::insertUser(User& user) {
        if(m_usersIn >= m_maxUsers)
            return -1;

        int userIndex = getFreeListIndex();
        m_roomUsers[userIndex].user = &user;
        ++m_usersIn;
        m_userQueue.push_back(userIndex);
		m_users.push_back(&user);

		if(m_state == RoomState::IN_GAME) {
			m_roomUsers[userIndex].inGameScreen = true;
			++m_inGameScreenUserCount;
		}

        updateRoomMaster();
        return userIndex;
    }

    void Room::removeUser(User& user) {
        int userIndex = getUserListIndex(user);
        removeUser(userIndex);
    }

    void Room::removeUser(int userListIndex) {
        if(userListIndex == -1)
            return;

        if(m_roomMasterListIndex == userListIndex)
            m_roomMasterListIndex = -1;

        RoomUser& roomUser = m_roomUsers[userListIndex];
        User* user = roomUser.user;
        if(m_state == RoomState::IN_GAME) {
            if(roomUser.playing == true) {
                roomUser.playing = false;
                --m_playingUserCount;
            }
            roomUser.inGameScreen = false;
            --m_inGameScreenUserCount;
        }

        //clearing slot
        if(roomUser.slot >= 0)
            setUserToSlot(-1, roomUser.slot);
        roomUser.slot = -1;
        roomUser.user = nullptr;
        --m_usersIn;

        auto itQueue = std::find(m_userQueue.begin(), m_userQueue.end(), userListIndex);
        m_userQueue.erase(itQueue);

        auto itUser = std::find(m_users.begin(), m_users.end(), user);
        m_users.erase(itUser);

        updateRoomMaster();
    }

    bool Room::isUserIn(const wchar_t *userName) {
        for(auto user : m_users) {
            if(user && ::wcscmp(userName, user->player()->name()) == 0)
                return true;
        }
        return false;
    }

	unsigned int Room::userCount() {
		return m_usersIn;
	}

    int Room::updateRoomMaster() {
        if(m_roomMasterListIndex != -1)
            return m_roomMasterListIndex;

        int next = geNextRoomMaster();
        if(next != -1)
            m_roomMasterListIndex = next;
        return next;
    }

    int Room::setUserToSlot(int listIndex, int slot) {
        if(listIndex == -1) {
            int userListIndex = m_slots.playerListIndex[slot];
            if(userListIndex != -1)
                m_roomUsers[userListIndex].slot = -1;
            m_slots.playerListIndex[slot] = -1;
            return 0;
        }

        if(m_slots.playerListIndex[slot] != -1)
            return 1;
        if(m_slots.state[slot] == 0)
            return 2;

        //clear current slot
        int userSlot = m_roomUsers[listIndex].slot;
        if(userSlot != -1)
            m_slots.playerListIndex[userSlot] = -1;
        m_roomUsers[listIndex].slot = slot;
        m_slots.playerListIndex[slot] = listIndex;
        return 0;
    }

    int Room::resetUserFromSlot(int userListIndex) {
        int slot = m_roomUsers[userListIndex].slot;
        if(slot != -1)
            m_slots.playerListIndex[slot] = -1;
        m_roomUsers[userListIndex].slot = -1;
        return 0;
    }

    void Room::resetUserFromSlots() {
        for(int i = 0; i < m_maxUsers; ++i)
            resetUserFromSlot(i);
    }

    int Room::setUserToSlot(User &user, int slot) {
        int listIndex = getUserListIndex(user);
        if(listIndex >= 0)
            return setUserToSlot(listIndex, slot);
        return 1;
    }

    int Room::setSlotState(int slot, Room::SlotState state) {
        if(m_slots.state[slot] == state)
            return 0;
        m_slots.playerListIndex[slot] = -1;
        m_slots.state[slot] = state;
        return 0;
    }

    int Room::setSlotStates(Room::SlotState states[ROOM_SLOT_COUNT]) {
        ::memcpy(m_slots.state, states, sizeof(SlotState) * ROOM_SLOT_COUNT);
        return 0;
    }

    User *Room::userInListIndex(int listIndex) const {
        return m_roomUsers[listIndex].user;
    }

    u32 Room::startMatch() {
        if(m_state != RoomState::IDLE)
            return 1;

        m_matchInfo = new Match(0, *this, 0);         //TODO(CGR): implement
        m_state = RoomState::IN_GAME;

        m_playingUserCount = 0;

		for(auto &user : m_roomUsers) {
			if (nullptr != user.user) {
				user.inGameScreen = true;
				user.playing = user.slot != -1;
                if(user.playing)
                    ++m_playingUserCount;
			}
		}

		m_inGameScreenUserCount = m_userQueue.size();
        return 0;
    }

    u32 Room::endMatch() {
        if(m_state != RoomState::IN_GAME)
            return 1;

        for(auto &roomUser : m_roomUsers) {
            if(nullptr != roomUser.user) {
                roomUser.playing = false;
            }
        }
        m_playingUserCount = 0;
        m_state = RoomState::IDLE;
        return 0;
    }

    void Room::setUserFinishedPlaying(User& user) {
        int listIndex = this->getUserListIndex(user);
        if(listIndex == -1)
            return;
        m_roomUsers[listIndex].playing = false;
        --m_playingUserCount;
    }

    void Room::setUserOutOfGameScreen(User& user) {
        int listIndex = this->getUserListIndex(user);
        if(listIndex == -1)
            return;
        m_roomUsers[listIndex].inGameScreen = false;
        --m_inGameScreenUserCount;
    }

}
