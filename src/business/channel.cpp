//
// Created by CGR on 16/05/2020.
//

#include "channel.h"

Channel::Channel(const wchar_t *channelName) {
    m_channelName = channelName;
}

void Channel::insertUser(User *user) {
    m_users.push_back(user);
}

void Channel::removeUser(User *user) {
    //TODO(CGR): A more elegant way to remove player from list
    auto it = m_users.begin();
    for(; it != m_users.end(); ++it) {
        User *u = *it;
        if(u == user) {
            m_users.erase(it);
            return;
        }
    }
}
