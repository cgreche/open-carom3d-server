//
// Created by CGR on 17/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_USER_H__
#define __OPEN_CAROM3D_SERVER_USER_H__

#include <string>
#include "user_spot.h"

namespace core { class ClientSession; }

namespace business {

    class GameServer;
    class Account;
    class Player;
    class UserSpot;
    class Channel;
    class Room;

    class User {
        core::ClientSession &m_clientSession;
        GameServer* m_server;
        Account *m_account;
        Player *m_player;
        UserSpot* m_spot;

        std::wstring m_lastChannelName;

    public:
        explicit User(core::ClientSession &session);

        void setServer(GameServer* server);
        void setAccount(Account* account);
        void setPlayer(Player* player);
        void setSpot(UserSpot *spot);

        core::ClientSession &client() const { return m_clientSession; }
        Account *account() const { return m_account; }
        Player *player() const { return m_player; }
        UserSpot *spot() const { return m_spot; }
        GameServer* server() const { return m_server; }

        const wchar_t* lastChannelName() const { return m_lastChannelName.c_str(); }

        //TODO(CGR): 0 to constant
        bool inChannel() const { return m_spot && m_spot->isOfType(0); }
        //TODO(CGR): 0 to constant
        bool inRoom() const { return m_spot && m_spot->isOfType(1); }

        Channel* channelIn() const { return inChannel() ? (Channel*)m_spot : nullptr; }
        Room* roomIn() const { return inRoom() ? (Room*)m_spot : nullptr; }
    };

}

#endif //__OPEN_CAROM3D_SERVER_USER_H__