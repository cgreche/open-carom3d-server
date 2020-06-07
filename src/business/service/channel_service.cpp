//
// Created by CGR on 16/05/2020.
//

#include <business/entity/player.h>
#include <business/util/abstract_action.h>
#include <business/util/action_dispatcher.h>
#include <business/util/destination.h>
#include "channel_service.h"

namespace business {

    static ChannelService g_lobbyService;

    ChannelService &ChannelService::getInstance() {
        return g_lobbyService;
    }

    Channel *ChannelService::createChannel(const wchar_t *channelName) {
        Channel *channel = getChannel(channelName);
        if (channel != nullptr)
            return channel;
        channel = new Channel(channelName);
        m_channels.push_back(channel);
        return channel;
    }

    Channel *ChannelService::getChannel(const wchar_t *channelName) {
        for (auto channel : m_channels) {
            if (::wcscmp(channel->name(), channelName) == 0)
                return channel;
        }
        return nullptr;
    }

    Channel *ChannelService::moveUserToChannel(User *user, const wchar_t *channelName, bool createIfNotExists) {
        Channel *channel = getChannel(channelName);
        if (channel == nullptr) {
            if (!createIfNotExists)
                return nullptr;
            channel = createChannel(channelName);
        }
        insertUserIntoChannel(*channel, *user);
        return channel;
    }

    void ChannelService::insertUserIntoChannel(Channel& channel, User &user) {
        //TODO(CGR): check if channel is not full
        channel.insertUser(user);
        user.setSpot(&channel);

        //TODO(CGR): modularize
        ChannelPlayer channelPlayer = {0};
        ActionData joinChannelAction(0x1D, (unsigned char *) channel.name(), (::wcslen(channel.name()) + 1) * 2);
        user.client().sendAction(joinChannelAction);

        channelPlayer.accountNumber = user.player()->id();
        ::wcsncpy(channelPlayer.playerName, user.player()->name(), 20);
        channelPlayer.playerName[20] = L'\0';
        channelPlayer.level = user.player()->level();

        for (User *userIn : channel.usersIn()) {
            ChannelPlayer channelPlayer = {0};
            channelPlayer.accountNumber = userIn->player()->id();
            channelPlayer.level = userIn->player()->level();
            ::wcscpy(channelPlayer.playerName, userIn->player()->name());
            ActionData channelPlayerAction(0x1E, (unsigned char *) &channelPlayer, sizeof(channelPlayer));
            user.client().sendAction(channelPlayerAction);
        }

        for (User *userIn : channel.usersIn()) {
            if(userIn == &user) continue;
            ActionData channelUserAction(0x1E, (unsigned char *) &channelPlayer, sizeof(channelPlayer));
            userIn->client().sendAction(channelUserAction);
        }

    }

    void ChannelService::removeUserFromChannel(Channel& channel, User &user) {
        channel.removeUser(user);
        user.setSpot(nullptr);

        //TODO(CGR): modularize
        unsigned long accountNumber = user.account()->id();
        ActionData userLeftChannelAction(0x1F, reinterpret_cast<unsigned char *>(&accountNumber), 4);
        ActionDispatcher::prepare().action(userLeftChannelAction).send(ChannelDestination(channel));
    }

    void ChannelService::sendUserMessage(Channel& channel, User &user, const wchar_t *message) {
        auto *_data = new unsigned char[(::wcslen(message) + 1) * 2 + 4];
        *(unsigned long *) &_data[0] = user.account()->id();
        ::wcscpy((wchar_t *) &_data[4], message);

        ActionData actionData(0x21, (unsigned char *) _data, (::wcslen(message) + 1) * 2 + 4);
        ActionDispatcher::prepare().action(actionData).send(ChannelDestination(channel));

        delete[] _data;
    }

}