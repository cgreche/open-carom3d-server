#pragma once

namespace nettools {

    class ntClient;

    class ntEventHandler {
    public:
        virtual ~ntEventHandler() { };
        virtual void onClientConnection(ntClient *client) { }
        virtual void onClientDisconnection(ntClient *client) { }
        virtual void onClientDataReceived(ntClient *client, unsigned char *data, unsigned int dataLen) { }
        virtual void onClientDataSent(ntClient* client, const unsigned char *data, unsigned int dataLen) { }
    };

}