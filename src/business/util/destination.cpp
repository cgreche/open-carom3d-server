//
// Created by CGR on 20/05/2020.
//

#include "destination.h"
#include <core/server/client_session.h>
#include <business/game_server/game_server.h>
#include <business/entity/user_spot.h>
#include <business/entity/channel.h>
#include <business/entity/room.h>
#include <business/entity/user.h>

namespace business {

    ServerDestination::ServerDestination(const GameServer &server, int to)
        : m_server(server)
        , m_to(to) {
    }

    void ServerDestination::send(const std::list<core::ActionData *> &actions) {
        auto const &clients = ((GameServer&)m_server).clients();
        for(auto const& client : clients) {
            User* user = (User*)client.second;
            //TODO(CGR): transform values in non-literal constants
            int to = m_to;
            if(to != 0) {
                UserSpot *spot = user->spot();
                if(nullptr == spot)
                    continue;
                if ((to & 1) != 0 && !spot->isOfType(0)) {
                    continue;
                }
                else if ((to & 2) != 0 && !spot->isOfType(1)) {
                    continue;
                }
            }

            for (auto action : actions)
                user->sendAction(*action);
        }
    }

    ChannelDestination::ChannelDestination(const Channel &channel)
            : m_channel(channel) {
    }

    void ChannelDestination::send(const std::list<core::ActionData *> &actions) {
        for (auto user : m_channel.usersIn()) {
            for (auto action : actions)
                user->sendAction(*action);
        }
    }

    RoomDestination::RoomDestination(const Room &room)
            : m_room(room) {

    }

    void RoomDestination::send(const std::list<core::ActionData *> &actions) {
        for (auto user : m_room.users()) {
            for (auto action : actions)
                user->sendAction(*action);
        }
    }

    UserDestination::UserDestination(const User &user)
            : m_user(user) {
    }

    void UserDestination::send(const std::list<core::ActionData *> &actions) {
        for (auto action : actions)
            ((User&)m_user).sendAction(*action);
    }

}
