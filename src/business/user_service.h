//
// Created by CGR on 16/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_USER_SERVICE_H__
#define __OPEN_CAROM3D_SERVER_USER_SERVICE_H__

#include <list>
#include <map>
#include <vector>
#include "player.h"
#include "channel.h"

class UserService {

    std::list<Account*> m_accounts;
    std::list<Player*> m_players;
    std::map<int, User*> m_clientsUsers;

    void linkClientToUser(ClientSession &client, User& user);

public:
    static UserService& getInstance();

    User *loginUser(wchar_t *acocuntId, wchar_t *accountPassword); //TODO(CGR): implement
    void logoutUser(User &user);

    User *createUserSession(ClientSession &clientSession, Account &account, Player &player); //TODO(CGR): deprecate
    User *getUser(ClientSession &clientSession);
    Channel *joinChannel(User &user, const wchar_t *channelName, bool createIfNotExists);
};

#endif //__OPEN_CAROM3D_SERVER_USER_SERVICE_H__
