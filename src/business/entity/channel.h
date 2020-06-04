//
// Created by CGR on 16/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_CHANNEL_H__
#define __OPEN_CAROM3D_SERVER_CHANNEL_H__

#include <string>
#include <vector>
#include "user_spot.h"

namespace business {

    class User;

    class Channel : public UserSpot {
        std::wstring m_channelName;
        std::vector<User *> m_users;

    public:
        explicit Channel(const wchar_t *channelName);

        bool isOfType(int type) override;
        const wchar_t* description() override;
        const wchar_t *name() override;

        int insertUser(User& user) override;
        void removeUser(User& user) override;
        bool isUserIn(const wchar_t* userName) override;
        unsigned int userCount() override;

        const wchar_t *name() const { return m_channelName.c_str(); }

        const std::vector<User *> &usersIn() const { return m_users; }
    };

}

#endif //__OPEN_CAROM3D_SERVER_CHANNEL_H__
