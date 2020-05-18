//
// Created by CGR on 17/05/2020.
//

#include <core/server/client_session.h>
#include "user_service.h"
#include "lobby_service.h"

static UserService g_userService;

UserService &UserService::getInstance() {
    return g_userService;
}

void UserService::linkClientToUser(ClientSession &client, User &user) {
    m_clientsUsers[client.sessionId()] = &user;
}

User* UserService::createUserSession(ClientSession& clientSession, Account& account, Player& player) {
    User* user = new User(clientSession, account, player);
    linkClientToUser(clientSession, *user);
    return user;
}

User *UserService::getUser(ClientSession &clientSession) {
    return m_clientsUsers[clientSession.sessionId()];
}

void UserService::logoutUser(User &user) {
    m_clientsUsers.erase(user.client().sessionId());
}

Channel *UserService::joinChannel(User &user, const wchar_t* channelName, bool createIfNotExists) {
    Channel* channel = LobbyService::getInstance().moveUserToChannel(&user, channelName, createIfNotExists);
    return channel;
}