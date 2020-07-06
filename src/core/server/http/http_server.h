//
// Created by CGR on 13/06/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_HTTP_SERVER_H__
#define __OPEN_CAROM3D_SERVER_HTTP_SERVER_H__

#include <map>
#include <core/server/server_context.h>

namespace core {

    class ClientSession;
    class Action;

    class HTTPServer : public Server {
    public:
        explicit HTTPServer(const ServerConfig &config);
        virtual ~HTTPServer() {};

        void run() override;

        MessagingProtocol* messagingProtocol() override;
    };

}

#endif //__OPEN_CAROM3D_SERVER_HTTP_SERVER_H__
