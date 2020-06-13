//
// Created by CGR on 09/06/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_CAROM3D_PROTOCOL_H__
#define __OPEN_CAROM3D_SERVER_CAROM3D_PROTOCOL_H__

#include <map>
#include <core/server/messaging_protocol.h>
#include "carom3d_message_parser.h"

namespace core {

    class Carom3DUserSession;
    class Action;

    class Carom3DProtocol : public MessagingProtocol {
    protected:
        Carom3DMessageParser m_messageParser;
        std::map<int, Action*>* m_userActionMap;

        void setUserActionMap(std::map<int, Action*>* actionMap);
        void processUserActions(Carom3DUserSession& session);

    public:
        Carom3DProtocol();

        ClientSession* createSession(nettools::ntConnection& ntClient, Server& server) override;
        void onMessageReceived(ClientSession& user) override;
        void onMessageSent(ClientSession& user) override;
        void closeSession(ClientSession& user) override;

        virtual void onUserAction(Carom3DUserSession& session, const ActionData& actionData);
        virtual void onUnhandledUserAction(Carom3DUserSession& session, const ActionData& actionData);
    };

}

#endif //__OPEN_CAROM3D_SERVER_CAROM3D_PROTOCOL_H__
