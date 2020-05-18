//
// Created by CGR on 16/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_PLAYER_H__
#define __OPEN_CAROM3D_SERVER_PLAYER_H__

#include <string>
#include "account.h"

class Account;

class Player {
    Account &m_account;

    std::wstring m_playerName;
    int m_points;
    int m_level;

public:
    Player(Account& account)
        : m_account(account) {
        m_playerName = account.name();
        m_points = 0;
        m_level = 0;
    }

    Account &account() { return m_account; }
    const wchar_t *name() const { return m_playerName.c_str(); }
    int points() const { return m_points; }
    int level() const { return m_level; }
};


#endif //__OPEN_CAROM3D_SERVER_PLAYER_H__
