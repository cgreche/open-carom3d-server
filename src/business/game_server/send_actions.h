//
// Created by CGR on 20/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_ACTIONS_H__
#define __OPEN_CAROM3D_SERVER_ACTIONS_H__

#include "game_server_action.h"
#include <core/server/carom3d/util/action_builder.h>

namespace business {

    class ActionTemplate {
    public:
        virtual ~ActionTemplate() {}
        virtual const ActionData data() = 0;
    };

    class PlayerProfileActionTemplate : public ActionTemplate {
        Player& m_player;

    public:
        explicit PlayerProfileActionTemplate(Player &player)
            : m_player(player) {
        }

        const ActionData data() override {
            PlayerProfileData data = { 0 };
            data.playerNumber = 0;
            ::wcscpy(data.playerName, m_player.name());
            ::wcscpy(data.playerGuild, L"");
            ::wcscpy(data.loggedServerName, L""); //TODO(CGR): get server name
            data.serverId = 0; //TODO(CGR): get server id
            ::wcscpy(data.country, L"BR"); //TODO(CGR): ip_to_country
            data.wins = 0 * 10;
            data.level = 1;
            return ActionData(0x3A, &data, sizeof(data));
        }
    };

    class UserPrivateMessageActionTemplate : public ActionTemplate {
        const wchar_t* m_userName;
        const wchar_t* m_message;

    public:
        explicit UserPrivateMessageActionTemplate(const wchar_t* userName, const wchar_t* message)
            : m_userName(userName), m_message(message) {
        }

        const ActionData data() override {
            return ActionBuilder(0x3B)
                .add(m_userName, (PLAYER_NAME_MAX_LEN + 1) * 2)
                .add(m_message)
                .build();
        }
    };

    class UserInviteActionTemplate : public ActionTemplate {
        User& m_user;
        Room& m_room;

    public:
        explicit UserInviteActionTemplate(User& user, Room& room)
            : m_user(user), m_room(room) {
        }

        const ActionData data() override {
            UserInvite userInvite;
            ::wcsstr(userInvite.playerName, m_user.player()->name());

            //TODO: complete
            RoomInfoActionData& roomInfo = userInvite.roomInfo;
            ::wcsstr(roomInfo.roomName, m_room.name());
            roomInfo.roomId = m_room.id();
            roomInfo.difficulty = m_room.gameInfo().difficulty;
            roomInfo.matchType = m_room.gameInfo().matchType;
            roomInfo.roomState = m_room.state();
            //

            ::wcsstr(userInvite.roomPassword, m_room.password());
            return ActionData(0x4B, &userInvite, sizeof(userInvite));
        }
    };

}


#endif //__OPEN_CAROM3D_SERVER_ACTIONS_H__
