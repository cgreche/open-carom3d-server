//
// Created by CGR on 20/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_ACTIONS_H__
#define __OPEN_CAROM3D_SERVER_ACTIONS_H__

#include <business/util/abstract_action.h>

namespace business {

    //0x01
	class LoginAction : public AbstractAction<LoginActionData> {
	public:
		void execute(const ActionData& action, User& user, const LoginActionData* data) override {
			UserService::getInstance().loginUser(user, data->username, data->password, data->country);
		}
	};

    class CreateRoomAction : public AbstractAction<CreateRoomActionData> {
        void execute(const ActionData &action, User &user, const CreateRoomActionData *data) override {
            UserService::getInstance().createRoom(user, *data);
        }
    };

	class JoinChannelAction : public AbstractAction<wchar_t> {
	public:
		void execute(const ActionData& action, User& user, const wchar_t* data) override {
			UserService::getInstance().joinChannel(user, data, false);
		}
	};

    class JoinRoomAction : public AbstractAction<JoinRoomActionData> {
        void execute(const ActionData &action, User &user, const JoinRoomActionData *data) override {
            UserService::getInstance().joinRoom(user, data->roomTitle, data->roomPassword);
        }
    };

    class ChannelMessageAction : public AbstractAction<wchar_t> {
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

    class ExitRoomAction : public AbstractAction<void> {
        void execute(const ActionData &action, User &user, const void *data) override {
            UserService::getInstance().exitRoom(user);
        }
    };

    class StartMatchAction : public AbstractAction<void> {
        void execute(const ActionData &action, User &user, const void *data) override {
            UserService::getInstance().startMatch(user);
        }
    };

    //0x0A
	class EndMatchAction : public AbstractAction<void> {
		void execute(const ActionData& action, User& user, const void* data) override {
            UserService::getInstance().matchFinished(user);
		}
	};

    class PlayerProfileRequestAction : public AbstractAction<wchar_t> {
    public:
        void execute(const ActionData &action, User &user, const wchar_t *playerName) override {
            PlayerProfileData data = {0};
            data.playerNumber = 0;
            ::wcscpy(data.playerName, L"GOD");
            ::wcscpy(data.playerGuild, L"");
            ::wcscpy(data.loggedServerName, L"Fake-1");
            data.serverId = 15000;
            data.country[0] = L'B';
            data.country[1] = L'R';
            data.wins = 30 * 10;
            data.level = 1;

            ActionData playerProfileAction(0x3A, &data, sizeof(data));
            user.client().sendAction(playerProfileAction);
        }
    };

    class MatchEventInfoAction : public AbstractAction<u8> {
    public:
        void execute(const ActionData &action, User &user, const u8 *data) override {
            UserService::getInstance().sendMatchEventInfo(user, data, action.data().size());
        }
    };


    class RoomSlotModificationAction : public AbstractAction<RoomSlotModificationActionData> {
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

    class RoomKickPlayerAction : public AbstractAction<int> {
        void execute(const ActionData &action, User &user, const int *data) override {
            UserService::getInstance().kickUserFromRoom(user, *data);
        }
    };

    class MatchMakerScreenRequestAction : public AbstractAction<wchar_t> {
        void execute(const ActionData &action, User &user, const wchar_t *data) override {
			UserService::getInstance().requestMatchMakerScreen(user);
        }
    };

    class RoomMessageAction : public AbstractAction<wchar_t> {
        void execute(const ActionData &action, User &user, const wchar_t *data) override {
            UserService::getInstance().sendMessageToRoom(user, data);
        }
    };

}


#endif //__OPEN_CAROM3D_SERVER_ACTIONS_H__
