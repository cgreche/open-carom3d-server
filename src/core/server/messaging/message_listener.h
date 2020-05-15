//
// Created by CGR on 13/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_MESSAGE_LISTENER_H__
#define __OPEN_CAROM3D_SERVER_MESSAGE_LISTENER_H__

#include <nettools/ntServer.h>
#include <nettools/ntClient.h>
#include "message_parser.h"

class Server;

class MessageListener {
    //
    Server &m_serverCtx;

    //
    nettools::ntServer m_ntServer;

    //
    MessageParser m_messageParser;

    void onClientConnection(nettools::ntClient *ntClient);
    void onClientDisconnection(nettools::ntClient *ntClient);
    void onClientDataReceived(nettools::ntClient *ntClient, unsigned char data[], unsigned int dataLen);
    void onClientDataSent(nettools::ntClient *ntClient, const unsigned char data[], unsigned int dataLen);

public:
    MessageListener(Server &server);
    void listen(unsigned short port);
};

#endif //__OPEN_CAROM3D_SERVER_MESSAGE_LISTENER_H__
