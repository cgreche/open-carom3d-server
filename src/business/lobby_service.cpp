//
// Created by CGR on 16/05/2020.
//

#include <business/player.h>
#include <core/server/client_session.h>
#include <core/server/action.h>
#include "lobby_service.h"

LobbyService g_lobbyService;

LobbyService & LobbyService::getInstance() {
    return g_lobbyService;
}

Channel* LobbyService::createChannel(const wchar_t* channelName) {
    Channel* channel = getChannel(channelName);
    if(channel != nullptr)
        return channel;
    channel = new Channel(channelName);
    m_channels.push_back(channel);
    return channel;
}

Channel* LobbyService::getChannel(const wchar_t* channelName) {
    for(auto channel : m_channels) {
        if(::wcscmp(channel->name(), channelName) == 0)
            return channel;
    }
    return nullptr;
}

Channel* LobbyService::moveUserToChannel(User *user, const wchar_t* channelName, bool createIfNotExists) {
    Channel* channel = getChannel(channelName);
    if(channel == nullptr) {
        if(!createIfNotExists)
            return nullptr;
        channel = createChannel(channelName);
    }
    channel->insertUser(user);
    user->setChannelIn(channel);
    m_playerToChannel[user->account().id()] = channel;
    return channel;
}

void LobbyService::removeUserFromPlace(User *user) {
    int id = user->account().id();
    Channel* channel = m_playerToChannel[id];
    if(nullptr != channel) {
        channel->removeUser(user);
        user->setChannelIn(nullptr);
    }

    for(auto userIn : channel->usersIn()) {
        unsigned long accountNumber = user->account().id();
        ActionData userLeftChannelAction(0x1F, reinterpret_cast<unsigned char *>(&accountNumber), 4);
        userIn->client().sendAction(userLeftChannelAction);
    }

}

void LobbyService::sendMessage(User *user, const wchar_t* message) {

    auto *_data = new unsigned char[(::wcslen(message)+1)*2 + 4];
    *(unsigned long*)&_data[0] = user->account().id();
    ::wcscpy((wchar_t*)&_data[4], message);

    Channel* channel = user->channelIn();
    ActionData actionData(0x21, (unsigned char*)_data, (::wcslen(message)+1)*2 + 4);
    for(User *userIn : channel->usersIn()) {
        userIn->client().sendAction(actionData);
    }

    delete [] _data;
}