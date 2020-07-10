//
// Created by CGR on 20/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_RECV_ACTIONS_H__
#define __OPEN_CAROM3D_SERVER_RECV_ACTIONS_H__

#include "game_server_action.h"

namespace business {

    //0x01
	class LoginAction : public GameServerAction<LoginActionData> {
	public:
		void execute(const ActionData& action, User& user, const LoginActionData* data) override {
			UserService::getInstance().loginUser(user, data->username, data->password, data->country);
		}
	};

    class CreateRoomAction : public GameServerAction<CreateRoomActionData> {
        void execute(const ActionData &action, User &user, const CreateRoomActionData *data) override {
            UserService::getInstance().createRoom(user, *data);
        }
    };

	class JoinChannelAction : public GameServerAction<wchar_t> {
	public:
		void execute(const ActionData& action, User& user, const wchar_t* data) override {
			UserService::getInstance().joinChannel(user, data);
		}
	};

    class JoinRoomAction : public GameServerAction<JoinRoomActionData> {
        void execute(const ActionData &action, User &user, const JoinRoomActionData *data) override {
            UserService::getInstance().joinRoom(user, data->roomTitle, data->roomPassword);
        }
    };

    class ChannelMessageAction : public GameServerAction<wchar_t> {
        bool validate(const ActionData &action) override {
            return true;
        }

        void execute(const ActionData &action, User &user, const wchar_t *message) override {
            //TODO(CGR): move to UserService
            UserSpot* spot = user.spot();
            if(!spot || !spot->isOfType(0))
                return;
            ChannelService::getInstance().sendUserMessage(*(Channel*)spot, user, message);
        }
    };

    class ExitRoomAction : public GameServerAction<void> {
        void execute(const ActionData &action, User &user, const void *data) override {
            UserService::getInstance().exitRoom(user);
        }
    };

    class StartMatchAction : public GameServerAction<void> {
        void execute(const ActionData &action, User &user, const void *data) override {
            UserService::getInstance().startMatch(user);
        }
    };

    //0x0A
	class EndMatchAction : public GameServerAction<void> {
		void execute(const ActionData& action, User& user, const void* data) override {
            UserService::getInstance().matchFinished(user);
		}
	};

    class PlayerProfileRequestAction : public GameServerAction<wchar_t> {
    public:
        void execute(const ActionData& action, User& user, const wchar_t* playerName) override {
            UserService::getInstance().requestPlayerProfile(user, playerName);
        }
    };

    class UserPrivateMessageAction : public GameServerAction<UserPrivateMessage> {
    public:
        void execute(const ActionData& action, User& user, const UserPrivateMessage* privateMessage) override {
            UserService::getInstance().sendPrivateMessageToUser(user, privateMessage->playerName, privateMessage->message);
        }
    };

    class UserSpotRequestAction : public GameServerAction<wchar_t> {
    public:
        void execute(const ActionData& action, User& user, const wchar_t* playerName) override {
            UserService::getInstance().requestUserSpot(user, playerName);
        }
    };

    class GuildProfileRequestAction : public GameServerAction<wchar_t> {
    public:
        void execute(const ActionData& action, User& user, const wchar_t* guildName) override {
            UserService::getInstance().requestGuildProfile(user, guildName);
        }
    };

    class GuildMessageAction : public GameServerAction<wchar_t> {
    public:
        void execute(const ActionData& action, User& user, const wchar_t* playerName) override {
            UserService::getInstance().sendGuildMessage(user, playerName);
        }
    };

    class GuildUserSpotsRequestAction : public GameServerAction<wchar_t> {
    public:
        void execute(const ActionData& action, User& user, const wchar_t* guildName) override {
            UserService::getInstance().requestGuildUserSpots(user, guildName);
        }
    };

    class SetCoverStatesAction : public GameServerAction<int> {
    public:
        void execute(const ActionData& action, User& user, const int states[]) override {
            UserService::getInstance().setCoverStates(user, states);
        }
    };

    //0x69
    class MatchEventInfoAction : public GameServerAction<u8> {
    public:
        void execute(const ActionData& action, User& user, const u8* data) override {
            UserService::getInstance().sendMatchEventInfo(user, data, action.data().size());
        }
    };

    //0x6B
    class MatchEventInfoAction2 : public GameServerAction<u8> {
    public:
        void execute(const ActionData& action, User& user, const u8* data) override {
            UserService::getInstance().sendMatchEventInfo2(user, data, action.data().size());
        }
    };

    class InviteUserToRoomAction : public GameServerAction<wchar_t> {
    public:
        void execute(const ActionData& action, User& user, const wchar_t* playerName) override {
            UserService::getInstance().inviteUserToRoom(user, playerName);
        }
    };

    class RoomSlotModificationAction : public GameServerAction<RoomSlotModificationActionData> {
        void execute(const ActionData &action, User &user, const RoomSlotModificationActionData *data) override {
            switch(data->function) {
                case 0: {
                    if(data->slotNumber >= 0)
                        UserService::getInstance().joinRoomSlot(user, data->slotNumber);
                    else
                        UserService::getInstance().exitRoomSlot(user);
                    break;
                }

                case 1:
                case 2: {
                    UserService::getInstance().setRoomSlotState(user, data->slotNumber, data->function);
                    break;
                }

                default: {
                    printf("Invalid slot modification function: %d", data->function);
                }

            }
        }
    };

    class RoomKickPlayerAction : public GameServerAction<int> {
        void execute(const ActionData &action, User &user, const int *data) override {
            UserService::getInstance().kickUserFromRoom(user, *data);
        }
    };

    class MatchMakerScreenRequestAction : public GameServerAction<wchar_t> {
        void execute(const ActionData &action, User &user, const wchar_t *data) override {
			UserService::getInstance().requestMatchMakerScreen(user);
        }
    };

    class RoomMessageAction : public GameServerAction<wchar_t> {
        bool validate(const ActionData& action) override {
            return action.data().size() < (151*sizeof(wchar_t));
        }

        void execute(const ActionData& action, User& user, const wchar_t* data) override {
            UserService::getInstance().sendMessageToRoom(user, data);
        }
    };

    class JoinIdAction : public GameServerAction<wchar_t> {
        void execute(const ActionData& action, User& user, const wchar_t* data) override {
            UserService::getInstance().joinUserRoom(user, data);
        }
    };

}


#endif //__OPEN_CAROM3D_SERVER_ACTIONS_H__
