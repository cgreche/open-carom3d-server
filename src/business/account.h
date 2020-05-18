//
// Created by CGR on 16/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_ACCOUNT_H__
#define __OPEN_CAROM3D_SERVER_ACCOUNT_H__

#include <string>

class ClientSession;
class Player;

class Account {
    long m_id;
    std::wstring m_name;
    std::wstring m_email;

    Player* m_player;

public:
    Account(int id, const wchar_t name[], const wchar_t email[]) {
        m_id = id;
        m_name = name;
        m_email = email;
        m_player = nullptr;
    }
    void setPlayer(Player* player) { m_player = player; };

    int id() const { return m_id; }
    const wchar_t *name() const { return m_name.c_str(); }
    const wchar_t *email() const { return m_email.c_str(); }
    Player* player() const { return m_player; }

};

#endif //__OPEN_CAROM3D_SERVER_ACCOUNT_H__
