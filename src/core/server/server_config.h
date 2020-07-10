//
// Created by CGR on 14/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_SERVER_CONFIG_H__
#define __OPEN_CAROM3D_SERVER_SERVER_CONFIG_H__

#include <string>

namespace core {

    struct ServerConfig {
        struct HostInfo {
            std::string hostname;
            unsigned short port;
        } hostInfo;
    };

}

#endif //__OPEN_CAROM3D_SERVER_SERVER_CONFIG_H__
