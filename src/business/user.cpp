//
// Created by Camila on 17/05/2020.
//

#include <core/server/client_session.h>
#include "account.h"
#include "player.h"
#include "user.h"

User::User(ClientSession &session, Account &account, Player &player)
    : m_clientSession(session)
    , m_account(account)
    , m_player(player) {
}

void User::setChannelIn(Channel *channel) {
    m_channelIn = channel;
}
