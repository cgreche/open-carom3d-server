//
// Created by CGR on 16/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_PLAYER_H__
#define __OPEN_CAROM3D_SERVER_PLAYER_H__

#include <string>
#include <core/util/types.h>
#include "account.h"

namespace business {

    class Account;

    class Player {
        Account &m_account;
        u32 m_id;
        std::wstring m_playerName;
        int m_points;
        int m_level;

    public:
        explicit Player(Account &account);

        Account &account() { return m_account; }
        u32 id() const { return m_id; }
        const wchar_t *name() const { return m_playerName.c_str(); }
        int points() const { return m_points; }
        int level() const { return m_level; }
    };

}

#endif //__OPEN_CAROM3D_SERVER_PLAYER_H__
