//
// Created by CGR on 17/05/2020.
//

#include <core/server/client_session.h>
#include <core/server/server_context.h>
#include "account.h"
#include "player.h"
#include "user.h"
#include "user_spot.h"

namespace business {

    User::User(nettools::ntConnection& ntConnection, core::Server& server)
        : Carom3DUserSession(ntConnection, server) {
        m_account = nullptr;
        m_player = nullptr;
        m_spot = nullptr;
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

    void User::setCoverStates(int coverStates[COVER_STATE_COUNT]) {
        m_coverStates[COVER_MESSAGE] = coverStates[COVER_MESSAGE];
        m_coverStates[COVER_GUILD] = coverStates[COVER_GUILD];
        m_coverStates[COVER_IN] = coverStates[COVER_IN];
    }

}