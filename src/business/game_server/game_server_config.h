//
// Created by CGR on 23/06/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_GAME_SERVER_CONFIG_H__
#define __OPEN_CAROM3D_SERVER_GAME_SERVER_CONFIG_H__

#include <list>
#include <core/server/carom3d/carom3d_server.h>
#include <business/api/constants.h>

namespace business {

#define GAME_TYPE_COUNT 8 //TODO(CGR): to correct

    struct GameServerGeneralConfig {
        int initialRating;
        int basePoints;
        int arcadeDifficultyMultiplier;
        int simuDifficultyMultiplier;
        int gameMultiplier[GAME_TYPE_COUNT];
    };

    enum GameServerEventType {
        NORMAL = 1
        //TODO(CGR): complete
    };

    struct GameServerTemplate {
        ServerTemplateId templateId;
        float baseCoefficient;
        int minPoints;
        int maxPoints;
    };

    struct GameServerConfig : public core::ServerConfig {
        GameServerEventType eventType;
        std::wstring serverName;
        int tableType;
        int serverId;
        GameServerTemplate serverTemplate;

    public:
        GameServerConfig(const core::ServerConfig& basicConfig,
            GameServerEventType eventType,
            std::wstring serverName,
            int tableType,
            int serverId,
            const GameServerTemplate& serverTemplate)
            : ServerConfig(basicConfig)
            , eventType(eventType)
            , serverName(serverName)
            , tableType(tableType)
            , serverId(serverId)
            , serverTemplate(serverTemplate) {
        }
    };

}

#endif //__OPEN_CAROM3D_SERVER_GAME_SERVER_CONFIG_H__
