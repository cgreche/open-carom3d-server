//
// Created by CGR on 16/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_LOBBY_SERVICE_H__
#define __OPEN_CAROM3D_SERVER_LOBBY_SERVICE_H__

#include <map>
#include <vector>
#include "player.h"
#include "channel.h"

class LobbyService {

    std::vector<Channel*> m_channels;
    std::map<int, Channel*> m_playerToChannel;

public:
    static LobbyService& getInstance();
    Channel *createChannel(const wchar_t *channelName);
    Channel *getChannel(const wchar_t* channelName);
    //TODO(CGR): Process channel operation errors, like "channel is full";
    Channel *moveUserToChannel(User *user, const wchar_t *channelName, bool createIfNotExists);
    void removeUserFromPlace(User* user);
    void sendMessage(User *user, const wchar_t *message);
};

#endif //__OPEN_CAROM3D_SERVER_LOBBY_SERVICE_H__
