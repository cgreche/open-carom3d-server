//
// Created by CGR on 14/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_SERVER_CONFIG_H__
#define __OPEN_CAROM3D_SERVER_SERVER_CONFIG_H__

#include <string>

namespace core {

    struct ServerConfig {
        std::string host;
        unsigned short port;
    };

}

#endif //__OPEN_CAROM3D_SERVER_SERVER_CONFIG_H__
