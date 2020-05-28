//
// Created by CGR on 13/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_MESSAGE_LISTENER_H__
#define __OPEN_CAROM3D_SERVER_MESSAGE_LISTENER_H__

#include <nettools/ntServer.h>
#include <nettools/ntClient.h>
#include <nettools/ntEventHandler.h>
#include "message_parser.h"

namespace core {

    class Server;

    class MessageListener {
        class EventHandler : public nettools::ntEventHandler {
            MessageListener &m_messageListener;

        public:
            EventHandler(MessageListener &messageListener)
                    : m_messageListener(messageListener) {
            }

            void onClientConnection(nettools::ntClient *client) override;

            void onClientDisconnection(nettools::ntClient *client) override;

            void onClientDataReceived(nettools::ntClient *client, unsigned char *data, unsigned int len) override;

            void onClientDataSent(nettools::ntClient *client, const unsigned char *data, unsigned int len) override;
        };

        //
        Server &m_serverCtx;

        //
        nettools::ntServer m_ntServer;

        EventHandler m_eventHandler;

        //
        MessageParser m_messageParser;

    public:
        MessageListener(Server &server);

        void listen(unsigned short port);

        void poll();
    };

}

#endif //__OPEN_CAROM3D_SERVER_MESSAGE_LISTENER_H__
