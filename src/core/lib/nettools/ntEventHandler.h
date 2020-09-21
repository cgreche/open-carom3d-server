#pragma once

namespace nettools {

    class ntConnection;

    class ntEventHandler {
    public:
        virtual ~ntEventHandler() { };
        virtual void onClientConnection(ntConnection *client) { }
        virtual void onClientDisconnection(ntConnection *client) { }
        virtual void onClientDataReceived(ntConnection*client, unsigned char *data, unsigned int dataLen) { }
        virtual void onClientDataSent(ntConnection* client, const unsigned char *data, unsigned int dataLen) { }
    };

}