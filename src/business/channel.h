//
// Created by CGR on 16/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_CHANNEL_H__
#define __OPEN_CAROM3D_SERVER_CHANNEL_H__

#include <string>
#include <vector>
#include "user.h"

class User;

class Channel {
    std::wstring m_channelName;
    std::vector<User*> m_users;

public:
    explicit Channel(const wchar_t* channelName);
    void insertUser(User *player);
    void removeUser(User *player);

    const wchar_t *name() const { return m_channelName.c_str(); }
    const std::vector<User*> &usersIn() const { return m_users; }

};

#endif //__OPEN_CAROM3D_SERVER_CHANNEL_H__
