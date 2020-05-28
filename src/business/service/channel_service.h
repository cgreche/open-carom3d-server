//
// Created by CGR on 16/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_CHANNEL_SERVICE_H__
#define __OPEN_CAROM3D_SERVER_CHANNEL_SERVICE_H__

#include <map>
#include <vector>
#include "business/entity/player.h"
#include "business/entity/channel.h"

namespace business {

    class ChannelService {

        std::vector<Channel *> m_channels;

    public:
        static ChannelService &getInstance();

        Channel *createChannel(const wchar_t *channelName);

        Channel *getChannel(const wchar_t *channelName);

        //TODO(CGR): Process channel operation errors, like "channel is full";
        Channel *moveUserToChannel(User *user, const wchar_t *channelName, bool createIfNotExists);
        void insertUserIntoChannel(Channel& channel, User &user);
        void removeUserFromChannel(Channel& channel, User &user);

        void sendUserMessage(Channel& channel, User &user, const wchar_t *message);
    };

}

#endif //__OPEN_CAROM3D_SERVER_LOBBY_SERVICE_H__
