//
// Created by CGR on 19/05/2020.
//

#include "room.h"
#include "match.h"

namespace business {

    int Room::getFreeListIndex() const {
        for (int i = 0; i < m_users.size(); ++i) {
            if (m_users[i].user == nullptr)
                return i;
        }
        return -1;
    }

    int Room::getUserListIndex(User &user) const {
        for (int i = 0; i < m_users.size(); ++i) {
            if (m_users[i].user == &user)
                return i;
        }
        return -1;
    }

    int Room::getUserSlot(User &user) const {
        int listIndex = getUserListIndex(user);
        if(listIndex >= 0)
            return m_users[listIndex].slot;
        return -1;
    }

    int Room::geNextRoomMaster() const {
        if (m_playerQueue.empty())
            return -1;
        return m_playerQueue[0];
    }

    Room::Room(GameServer& server, u32 id, const wchar_t *title, User *creator, u32 maxPlayers, const Room::GameInfo &gameInfo)
        : m_server(server)
		, m_users(maxPlayers) {
        m_id = id;
        m_title = title;
        m_creator = creator;
        m_roomMaster = nullptr;
        m_maxPlayers = maxPlayers;
        m_gameInfo = gameInfo;
        m_closed = false;
        m_matchInfo = nullptr;
        m_straightWins = 0;
		m_inGameUserCount = 0;

        //init user info
        for(auto &user : m_users) {
            user.user = nullptr;
            user.slot = -1;
			user.inGame = false;
        }

        //initializing slots
        for(int i = 0; i < 30; ++i) {
            m_slots.state[i] = SlotState::DISABLED;
            m_slots.playerListIndex[i] = -1;
        }

//        if(nullptr != creator) {
//            int listIndex = insertUser(*creator);
//            setUserToSlot(ROOM_MASTER_SLOT, listIndex);
//        }
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
        if (m_playersIn >= m_maxPlayers)
            return -1;

        int userIndex = getFreeListIndex();
        m_users[userIndex].user = &user;
        ++m_playersIn;
        m_playerQueue.push_back(userIndex);

		if (m_state == RoomState::IN_GAME)
			++m_inGameUserCount;

        updateRoomMaster();
        return userIndex;
    }

    void Room::removeUser(User& user) {
        int userIndex = getUserListIndex(user);
        removeUser(userIndex);
    }

    void Room::removeUser(int userListIndex) {
        if(userListIndex >= 0) {
            if(m_roomMaster == m_users[userListIndex].user)
                m_roomMaster = nullptr;

                //clearing slot
            if(m_users[userListIndex].slot >= 0)
                setUserToSlot(m_users[userListIndex].slot, -1);
            m_users[userListIndex].slot = -1;
            m_users[userListIndex].user = nullptr;

            --m_playersIn;
			if (m_state == RoomState::IN_GAME)
				--m_inGameUserCount;

            //TODO(CGR): cleaner way to remove user
            for(auto it = m_playerQueue.begin(); it != m_playerQueue.end(); ++it)
                if(*it == userListIndex) {
                    m_playerQueue.erase(it);
                    break;
                }

            updateRoomMaster();
        }
    }

    bool Room::isUserIn(const wchar_t *userName) {
        for(auto &roomUser : m_users) {
            User* user = roomUser.user;
            if(user && ::wcscmp(userName, user->player()->name()) == 0)
                return true;
        }
        return false;
    }

    int Room::updateRoomMaster() {
        if (m_roomMaster != nullptr) {
            return -1;
        }

        int next = geNextRoomMaster();
        if (next != -1) {	
            setUserToSlot(ROOM_MASTER_SLOT, next);
            m_roomMaster = m_users[next].user;
        }
        return next;
    }

    int Room::setUserToSlot(int slot, int listIndex) {
        if(listIndex == -1)
            return resetUserFromSlot(slot);

        if(m_slots.playerListIndex[slot] != -1)
            return 1;
        if(m_slots.state[slot] == 0)
            return 2;

        //clear current slot
        int userSlot = m_users[listIndex].slot;
        if(userSlot != -1)
            m_slots.playerListIndex[userSlot] = -1;
        m_users[listIndex].slot = slot;
        m_slots.playerListIndex[slot] = listIndex;
        return 0;
    }

    int Room::resetUserFromSlot(int slot) {
        int slotListIndex = m_slots.playerListIndex[slot];
        if(slotListIndex != -1)
            m_users[slotListIndex].slot = -1;
        m_slots.playerListIndex[slot] = -1;
        return 0;
    }

    int Room::setUserToSlot(int slot, User &user) {
        int listIndex = getUserListIndex(user);
        if(listIndex >= 0)
            return setUserToSlot(slot, listIndex);
        return 1;
    }

    int Room::setSlotState(int slot, Room::SlotState state) {
        if(m_slots.state[slot] == state)
            return 0;
        //TODO(CGR): check if there are users in slot
        m_slots.playerListIndex[slot] = -1;
        m_slots.state[slot] = state;
        return 0;
    }

	void Room::resetSlotPlayerIndexes() {
		for (int i = 0; i < ROOM_SLOT_COUNT; ++i)
			m_slots.playerListIndex[i] = -1;
	}

    User *Room::userInListIndex(int listIndex) const {
        return m_users[listIndex].user;
    }

    u32 Room::startGame() {
        //TODO(CGR): implement
        m_matchInfo = new Match(0, *this, 0, 0, 0);
        m_state = RoomState::IN_GAME;
		for (auto user : m_users) {
			if(nullptr != user.user)
				user.inGame = true;
		}
		m_inGameUserCount = m_playerQueue.size();
        return 0;
    }

	void Room::setUserOutGame(User& user) {
		int listIndex = this->getUserListIndex(user);
		if (listIndex == -1)
			return;
		m_users[listIndex].inGame = false;
		--m_inGameUserCount;
	}

}
