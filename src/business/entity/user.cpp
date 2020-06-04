//
// Created by CGR on 17/05/2020.
//

#include <core/server/client_session.h>
#include "account.h"
#include "player.h"
#include "user.h"
#include "user_spot.h"

namespace business {

    User::User(core::ClientSession &session)
        : m_clientSession(session) {
        m_server = nullptr;
        m_account = nullptr;
        m_player = nullptr;
        m_spot = nullptr;
    }

    void User::setServer(GameServer *server) {
        m_server = server;
    }

    void User::setAccount(Account *account) {
        m_account = account;
    }

    void User::setPlayer(Player *player) {
        m_player = player;
    }

    void User::setSpot(UserSpot *spot) {
        if(spot && spot->isOfType(0))
            m_lastChannelName = spot->name();
        m_spot = spot;
    }

}