//
// Created by CGR on 20/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_DESTINATION_H__
#define __OPEN_CAROM3D_SERVER_DESTINATION_H__

#include "action_dispatcher.h"

namespace business {

    class GameServer;
    class Channel;
    class Room;
    class User;

    class ServerDestination : public core::Destination {
        const GameServer &m_server;
        int m_to;
    public:
        explicit ServerDestination(const GameServer &server, int to = 0);
        void send(const std::list<core::ActionData*>& actions) final;
    };

    class ChannelDestination : public core::Destination {
        const Channel &m_channel;
    public:
        explicit ChannelDestination(const Channel &channel);
        void send(const std::list<core::ActionData*>& actions) final;
    };

    class RoomDestination : public core::Destination {
        const Room &m_room;
    public:
        explicit RoomDestination(const Room &room);
        void send(const std::list<core::ActionData*>& actions) final;
    };

    class UserDestination : public core::Destination {
        const User &m_user;
    public:
        explicit UserDestination(const User &user);
        void send(const std::list<core::ActionData*>& actions) final;
    };

}

#endif //__OPEN_CAROM3D_SERVER_DESTINATION_H__
