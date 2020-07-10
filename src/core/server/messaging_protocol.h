//
// Created by CGR on 09/06/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_MESSAGING_PROTOCOL_H__
#define __OPEN_CAROM3D_SERVER_MESSAGING_PROTOCOL_H__

#include <vector>
#include <nettools/ntConnection.h>

namespace core {
    class ClientSession;
    class Server;

    class MessagingProtocol {
    public:
        virtual ~MessagingProtocol() {}

        virtual ClientSession* createSession(nettools::ntConnection& ntClient, Server& server) = 0;
        virtual void onMessageReceived(ClientSession& session) = 0;
        virtual void onMessageSent(ClientSession& session) = 0;
        virtual void closeSession(ClientSession& session) = 0;
    };

}

#endif //__OPEN_CAROM3D_SERVER_MESSAGING_PROTOCOL_H__
