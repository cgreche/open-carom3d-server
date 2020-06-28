//
// Created by CGR on 19/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_CONSTANTS_H__
#define __OPEN_CAROM3D_SERVER_CONSTANTS_H__

#define ACCOUNT_NAME_MAX_LEN 20
#define ACCOUNT_PASSWORD_MAX_LEN 12
#define ACCOUNT_COUNTRY_MAX_LEN 3

#define PLAYER_NAME_MAX_LEN 20
#define GUILD_NAME_MAX_LEN 16

#define ROOM_TITLE_MAX_LEN 40
#define ROOM_PASSWORD_MAX_LEN 8
#define ROOM_SLOT_COUNT 30

enum class SpotType {
    LOGGED_OFF = 0,
    CHANNEL = 1,
    ROOM = 2,
    IN_GAME = 3
};

enum MatchType {
    MATCH_NORMAL = 0,
    MATCH_GUILD_GAME55,
    MATCH_GUILD_GAME66,
    MATCH_GUILD_GAME33,
    MATCH_CHALLENGE,
    MATCH_PRACTICE = 5
};

enum GameType {
    GAME_NONE,
    GAME_3BALL,
    GAME_4BALL,
    GAME_6BALL,
    GAME_9BALL,
    GAME_8BALL,
    GAME_ROTATION,
    GAME_DEATH_MATCH_NORMAL,
    GAME_DEATH_MATCH_HIGH,
    GAME_4BALL_PRO,
    GAME_3BALL_PRO,
    GAME_CARDBALL_NORMAL,
    GAME_CARDBALL_HIGH,
    GAME_DEATH_MATCH_U,
    GAME_DEATH_MATCH_U2,
    GAME_SNOOKER_SHORT,
    GAME_SNOOKER_NORMAL,
    GAME_6BALL_PRO
};

enum Level {
    BRONZE_CHALK,
    SILVER_CHALK,
    GOLD_CHALK,
    BRONZE_RING,
    SILVER_RING,
    GOLD_RING,
    BRONZE_MEDAL,
    SILVER_MEDAL,
    GOLD_MEDAL,
    BRONZE_TROPHY,
    SILVER_TROPHY,
    GOLD_TROPHY,
    BRONZE_LAUREL_CROWN,
    SILVER_LAUREL_CROWN,
    GOLD_LAUREL_CROWN,
    BRONZE_CROWN,
    SILVER_CROWN,
    GOLD_CROWN,

    LEVEL_COUNT
};

enum ServerTemplateId {
    TRAINING = 0,
    FREE = 1,
    NOVICE = 2,
    ADVANCED = 3,
    EXPERT = 4,
    TOURNAMENT = 5,
    PROFESSIONAL = 6,
    RANKING = 7,
    WCGLEAGUE = 8,
    LEAGUE = 9
};

enum TableType {
    POCKET,
    NO_POCKET,
    SNOOKER
};

#define DEFAULT_BASE_POINTS 6

#endif //__OPEN_CAROM3D_SERVER_CONSTANTS_H__
