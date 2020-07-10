//
// Created by CGR on 19/05/2020.
//

#include <algorithm>
#include "room.h"
#include "match.h"

namespace business {
    extern const int NORMAL_GAME_SLOT_LAYOUT[ROOM_SLOT_COUNT] = {
        1, 1, 1, 0, 0, 0,
        1, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0
    };

    extern const int DEATH_MATCH_SLOT_LAYOUT[ROOM_SLOT_COUNT] = {
        1, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0
    };

    extern const int CARD_BALL_SLOT_LAYOUT[ROOM_SLOT_COUNT] = {
        1, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0
    };

    extern const int CHALLENGE_ROOM_SLOT_LAYOUT[ROOM_SLOT_COUNT] = {
        1, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0
    };

}
